/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "DMA.h"

uint32_t set_bit(uint32_t num, int b, bool v) {
    if (v) num |= (1 << b);
    else num &= ~(1 << b);

    return num;
}

bool DMA::is_channel_enabled(DMAChannels channel) {
    Logging console;
    console.warn("DMA IS_CHANNEL_ENABLED IS NOT FULLY IMPLEMENTED");
    return true;
}
void DMA::transfer_finished(DMAChannels dma_channel) {
    DMAChannel& channel = channels[(int)dma_channel];

    if (irq.enable & (1 << (int)dma_channel) || irq.master_enable)
        irq.flags = set_bit(irq.flags, (int)dma_channel, true);

    bool previous = irq.master_flag;
    irq.master_flag = irq.force || (irq.master_enable && ((irq.enable & irq.flags) > 0));

    if (irq.master_flag && !previous) {
        irq_pending = true;
    }
}

void DMA::start(DMAChannels dma_channel) {
    DMAChannel& channel = channels[(uint32_t)dma_channel];
    if (channel.control.sync_mode == SyncType::Linked_List)
        /* Start linked list copy routine. */
        list_copy(dma_channel);
    else
        /* Start block copy routine. */
        block_copy(dma_channel);

    /* Complete the transfer. */
    transfer_finished(dma_channel);
}
void DMA::block_copy(DMAChannels dma_channel) {
    DMAChannel& channel = channels[(uint32_t)dma_channel];

    uint32_t trans_dir = channel.control.trans_dir;
    SyncType sync_mode = channel.control.sync_mode;
    uint32_t step_mode = channel.control.addr_step;

    uint32_t base_addr = channel.base;

    int32_t increment = 0;
    switch (step_mode) {
        case 0:
            increment = 4;
            break;
        case 1:
            increment = -4;
            break;
    }

    uint32_t block_size = channel.block.block_size;
    if (sync_mode == SyncType::Request)
        block_size *= channel.block.block_count;

    while (block_size > 0) {
        uint32_t addr = base_addr & 0x1ffffc;

        switch (trans_dir) {
            case 0: {
                uint32_t data = 0;

                switch (dma_channel) {
                    case DMAChannels::OTC:
                        data = (block_size == 1 ? 0xffffff :
                                (addr - 4) & 0x1fffff);
                        break;
                    case DMAChannels::GPU:
                        data = gpu.get_gpuread();
                        break;
                    case DMAChannels::CDROM:
                        //data = bus->cddrive->read_word();
                        printf("PLEASE IMPLEMENT THE CD DRIVE!!!\n");
                        break;
                    default:
                        printf("Unhandled DMA source channel: 0x%x\n", dma_channel);
                }

                memory->writeWord(addr, data);
                break;
            }
            case 1: {
                uint32_t command = memory->readWord(addr);

                switch (dma_channel) {
                    case DMAChannels::GPU:
                        gpu.write_gp0(command);
                        break;
                    default:
                        break;
                }
                break;
            }
        }

        base_addr += increment;
        block_size--;
    }

    channel.control.enable = false;
    channel.control.trigger = false;
}
void DMA::list_copy(DMAChannels dma_channel) {
    DMAChannel& channel = channels[(uint32_t)dma_channel];
    uint addr = channel.base & 0x1ffffe;

    /* TODO: implement Device to Ram DMA transfer. */
    if (channel.control.trans_dir == 0) {
        printf("Not supported DMA direction!\n");
    }

    /* While not reached the end. */
    while (true) {
        /* Get the list packet header. */
        ListPacket packet;
        packet.raw = memory->readWord(addr);
        uint count = packet.size;

        /*if (count > 0)
            printf("Packet size: %d\n", count);*/

        /* Read words of the packet. */
        while (count > 0) {
            /* Point to next packet address. */
            addr = (addr + 4) & 0x1ffffc;

            /* Get command from main RAM. */
            uint command = memory->readWord(addr);

            /* Send data to the GPU. */
            gpu.write_gp0(command);
            count--;
        }

        /* If address is 0xffffff then we are done. */
        /* NOTE: mednafen only checks for the MSB, but I do no know why. */

        if (packet.next_addr & (1 << 23))
            break;

        /* Mask address. */
        addr = packet.next_addr & 0x1ffffc;
    }

    /* Complete DMA Transfer */
    channel.control.enable = false;
    channel.control.trigger = false;
}

uint32_t DMA::read(uint32_t address) {
    uint offset = address - 0x1f801080;

    /* Get channel information from address. */
    uint channel_num = (offset & 0x70) >> 4;
    uint reg = offset & 0xf;

    /* One of the main channels is enabled. */
    if (channel_num >= 0 && channel_num <= 6) {
        DMAChannel& channel = channels[channel_num];

        switch (reg) {
            case 0:
                return channel.base;
            case 4:
                return channel.block.raw;
            case 8:
                return channel.control.raw;
            default:
                printf("[DMA] DMAController::read: unhandled channel read at offset: 0x%x\n", offset);
                exit(1);
        }
    } /* One of the primary registers is selected. */
    else if (channel_num == 7) {

        switch (reg) {
            case 0:
                return control;
            case 4:
                return irq.raw;
            default:
                printf("[DMA] DMAController::read: unhandled read at offset: 0x%x\n", offset);
                exit(1);
        }
    }

    return 0;
}
void DMA::write(uint32_t address, uint32_t val) {
    // Just directly provide the address no need to subtract.
    uint offset = address - 0x1f801080;

    /* Get channel information from address. */
    uint channel_num = (offset & 0x70) >> 4;
    uint reg = offset & 0xf;

    uint active_channel = INT_MAX;
    /* One of the main channels is enabled. */
    if (channel_num >= 0 && channel_num <= 6) {
        DMAChannel& channel = channels[channel_num];

        switch (reg) {
            case 0:
                channel.base = val & 0xffffff;
                break;
            case 4:
                channel.block.raw = val;
                break;
            case 8:
                channel.control.raw = val;
                break;
            default:
                printf("[DMA] DMAController::write: unhandled channel write at offset: 0x%x\n", offset);
                exit(1);
        }

        /* Check if the channel was just activated. */
        bool trigger = true;
        if (channel.control.sync_mode == SyncType::Manual)
            trigger = channel.control.trigger;

        if (channel.control.enable && trigger)
            active_channel = channel_num;
    }/* One of the primary registers is selected. */
    else if (channel_num == 7) {

        switch (reg) {
            case 0:
                control = val;
                break;
            case 4:
                irq.raw = val;
                irq.master_flag = irq.force || (irq.master_enable && ((irq.enable & irq.flags) > 0));
                break;
            default:
                printf("[DMA] DMAController::write: unhandled write at offset: 0x%x\n", offset);
                exit(1);
        }
    }

    /* Start DMA if a channel was just activated. */
    if (active_channel != INT_MAX)
        start((DMAChannels)active_channel);
}

void DMA::tick() {
    if (irq_pending) {
        irq_pending = false;
        registers->i_stat |= (1 << (uint32_t)3);
    }
}