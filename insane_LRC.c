#include <linux/module.h>
#include <linux/slab.h>
#include "insane.h"

#include "lrc_config.c"

static struct parity_places algorithm_lrc( struct insane_c *ctx, u64 block, sector_t *sector, int *device_number );
static int lrc_configure( struct insane_c *ctx );

static struct recover_stripe recover_lrc(struct insane_c *ctx, u64 block, int device_number);

struct insane_algorithm lrc_alg = {
	.name       = "lrc",
	.p_blocks   = SUBSTRIPES + GLOBAL_S,
	.e_blocks   = E_BLOCKS,
	.stripe_blocks = (SUBSTRIPE_DATA + 1) * SUBSTRIPES + E_BLOCKS + GLOBAL_S,
	.map        = algorithm_lrc,
        .recover    = recover_lrc,
	.configure  = lrc_configure,
    	.module     = THIS_MODULE
};

/*
 * LRC RAID algorithm
 */
static struct parity_places algorithm_lrc( struct insane_c *ctx, u64 block, sector_t *sector, int *device_number )
{
	struct parity_places parity;
	
	u64 virtual_stripe;	
	u64 vs_position;
	u64 data_block, lane_pos;
	u64 global_gap, local_gap;
	u64 last_block;

	sector_t local_parity, global_parity;
	int block_size;
	int total_disks;
	int i,j;
        unsigned char group;

	block_size = ctx->chunk_size;
	total_disks = lrc_alg.ndisks;

        // number of data block (block in raid which is not empty or syndrome)
	data_block = *device_number + block * total_disks;

	/*
	 * Let's get position of block in VS.
	 * SUBSTRIPE_DATA - quantity of data blocks in substripe;
	 * SUBSTRIPES - quantity of substripes in VS;
	 */
	
	virtual_stripe = data_block;
	vs_position = sector_div(virtual_stripe, (SUBSTRIPE_DATA * SUBSTRIPES)); 
	// virtual_stripe  	- number of vs.
	// vs_position 		- offset in vs without counting parity.

	// Now let's get positions of syndromes.
        group = lrc_data[vs_position];
        local_parity = lrc_ls[group];
	local_parity = (virtual_stripe * lrc_alg.stripe_blocks) + local_parity;
	//global_parity = virtual_stripe * lrc_alg.stripe_blocks + lrc_gs;
	
	// Parity in sequential	mode
	if (ctx->io_pattern == SEQUENTIAL)
	{
		parity.start_sector = virtual_stripe * lrc_alg.stripe_blocks;
		parity.start_device = sector_div(parity.start_sector, total_disks);
		parity.start_sector = parity.start_sector * block_size;

		for (i = 0; i < SUBSTRIPES; i++) {
                        // local syndromes
			parity.device_number[i] = (parity.start_device + lrc_ls[i]) % ctx->ndev;
			if (parity.device_number[i] < parity.start_device) {
				parity.sector_number[i] = parity.start_sector + ctx->chunk_size;
			} else {
				parity.sector_number[i] = parity.start_sector;
			}
		}
                // global syndromes
                for (j = 0; i < SUBSTRIPES + GLOBAL_S; i++) {
    		    parity.device_number[i] = (parity.start_device + lrc_gs[j]) % ctx->ndev;

		    if (parity.device_number[i] < parity.start_device) {
			parity.sector_number[i] = parity.start_sector + ctx->chunk_size;
        	    } else {
	    		parity.sector_number[i] = parity.start_sector;
	            }
                    
                    j++;
                }
                // breakpoint
		parity.device_number[i] = -1;
	
		last_block = parity.start_device + lrc_ldb;
		i = sector_div(last_block, total_disks);
		last_block = i;
	}
	// Parity in random mode
	else {	
		parity.device_number[0] = sector_div(local_parity, total_disks);
		parity.sector_number[0] = local_parity * block_size;

                for (i = 0; i < GLOBAL_S; i++) {
                    global_parity = virtual_stripe * lrc_alg.stripe_blocks + lrc_gs[i];
                    parity.device_number[i+1] = sector_div(global_parity, total_disks);
                    parity.sector_number[i+1] = global_parity * block_size;    
                }

	        //breakpoint
                parity.device_number[i+1] = -1;

		parity.start_sector = virtual_stripe * lrc_alg.stripe_blocks;
		parity.start_device = sector_div(parity.start_sector, total_disks);
		parity.start_sector = parity.start_sector * block_size;
	}

	// Okay, let's return to counting position of data block
	// now we can calculate global_gap...
	global_gap = virtual_stripe * (SUBSTRIPES + E_BLOCKS + GLOBAL_S);
	
	// ...and local_gap
	local_gap = vs_position;
        i = 0;
        while (i < SUBSTRIPES + E_BLOCKS + GLOBAL_S) {
            if (local_gap >= lrc_offset[i]) {
                local_gap++;
            }
            i++;
        }

	// Almost ready.
	lane_pos = data_block + global_gap + local_gap - vs_position;
	/*
	 * Finally, we can get new device_number and sector number from lane_pos.
	 */

	*device_number = sector_div(lane_pos, total_disks);

	parity.last_block = false;
	if (ctx->io_pattern == SEQUENTIAL)
	{
	    if (*device_number == last_block)
		parity.last_block = true;
	}
	
	i = sector_div(*sector, block_size);
	*sector = lane_pos * block_size + i;
        

	return parity;
}


static struct recover_stripe recover_lrc(struct insane_c *ctx, u64 block, int device_number) {
    struct recover_stripe result;

    int total_disks, i, j, block_in_stripe;
    u64 chunk_size, stripe_number, sector, substripe_number, empty_device;

    bool gs;

    unsigned char pattern;

    total_disks = lrc_alg.ndisks;
    chunk_size = ctx->chunk_size;

    // calculating stripe number
    stripe_number = block * total_disks + device_number;
    block_in_stripe = sector_div(stripe_number, lrc_alg.stripe_blocks);

    // GLOBAL SYNDROME case
    gs = false;
    for (i = 0; i < GLOBAL_S; i++) {
        if (block_in_stripe == lrc_gs[i]) {
            gs = true;
            break;
        }
    }

    if (gs) {
        substripe_number = 0;
        j = 0;
        for (i = 0; i < lrc_alg.stripe_blocks; i++) {
            if (lrc_scheme[i] < 16) {
                // calculating read parameteres
                sector = stripe_number * lrc_alg.stripe_blocks + i; 
                result.read_device[j] = sector_div(sector, total_disks);
                result.read_sector[j] = sector * chunk_size;
                j++;
            }
        }

        result.quantity = lrc_alg.stripe_blocks - GLOBAL_S - SUBSTRIPES - E_BLOCKS;
        
        result.write_device = device_number - block_in_stripe + lrc_eb;
        result.write_sector = block * chunk_size;
        
        // it is possible to comment this clause, if you have checked your scheme
        if (result.write_device < 0) {
            result.write_device += total_disks;
            result.write_sector -= chunk_size;
        }
	
        return result;

    }

    // EMPTY BLOCK case
    if (block_in_stripe == lrc_eb) {
        result.quantity = 0;
        result.write_device = -1;

        return result;
    }

    // other cases

    // calculating substripe number
    substripe_number = lrc_scheme[block_in_stripe] | 0xc0; // local syndrome of substripe

    j = 0;

    for (i = 0; i < lrc_alg.stripe_blocks; i++) {
        pattern = lrc_scheme[i] | 0xc0; // now pattern can be local syndrome of group, global syndrome or empty block.
        
        if ((pattern == substripe_number) && (i != block_in_stripe)) {
            sector = stripe_number * lrc_alg.stripe_blocks + i;
            result.read_device[j] = sector_div(sector, total_disks);
            result.read_sector[j] = sector * chunk_size;
            j++;
        }
    }

    empty_device = device_number - block_in_stripe + lrc_eb + total_disks; // device with empty block

    result.write_device = sector_div(empty_device, total_disks);

    if (i < device_number)  // empty block is on the next lane
        result.write_sector = (block + 1) * chunk_size;
    else                    // empty block in on the current lane
        result.write_sector = block * chunk_size;
	
    result.quantity = SUBSTRIPE_DATA;
        
    return result;
}

static int lrc_configure( struct insane_c *ctx )
{
	if (!ctx)
		return -EINVAL;

	lrc_alg.ndisks = ctx->ndev;
	return 0;
}

static int __init insane_lrc_init( void )
{
	int r;
	
	r = insane_register( &lrc_alg );
	if (r)
		return r;
	
	return 0;
}

static void __exit insane_lrc_exit( void )
{
	insane_unregister( &lrc_alg );
}

module_init(insane_lrc_init);
module_exit(insane_lrc_exit);

MODULE_AUTHOR("Evgeniy Anastasiev");
MODULE_LICENSE("GPL");
