// SPDX-License-Identifier: GPL-2.0
/* Minimal XDP RX queue registration for the 4.14.336 tree. */
#include <linux/netdevice.h>
#include <net/xdp.h>

int xdp_rxq_info_reg(struct xdp_rxq_info *xdp_rxq,
		     struct net_device *dev, u32 queue_index)
{
	xdp_rxq->dev = dev;
	xdp_rxq->queue_index = queue_index;
	xdp_rxq->reg_state = 1;
	return 0;
}
EXPORT_SYMBOL_GPL(xdp_rxq_info_reg);

void xdp_rxq_info_unreg(struct xdp_rxq_info *xdp_rxq)
{
	xdp_rxq->reg_state = 0;
	xdp_rxq->dev = NULL;
}
EXPORT_SYMBOL_GPL(xdp_rxq_info_unreg);

void xdp_return_frame(struct xdp_frame *xdpf) { }
void xdp_return_frame_rx_napi(struct xdp_frame *xdpf) { }
void __xdp_release_frame(void *data, struct xdp_mem_info *mem) { }
void xdp_warn(const char *msg, const char *func, const int line) { pr_warn_once("XDP: %s at %s:%d\n", msg, func, line); }
EXPORT_SYMBOL_GPL(xdp_return_frame);
EXPORT_SYMBOL_GPL(xdp_return_frame_rx_napi);
EXPORT_SYMBOL_GPL(__xdp_release_frame);
EXPORT_SYMBOL_GPL(xdp_warn);

void xdp_return_buff(struct xdp_buff *xdp) { }
EXPORT_SYMBOL_GPL(xdp_return_buff);
