// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/pid.h>
#include <soc/oplus/oppo_healthinfo.h>

bool __weak selinux_is_enabled(void) { return false; }
int __weak sched_forked_ramup_factor(void) { return 100; }
bool __weak ux_task_misfit(struct task_struct *p, int cpu) { return false; }
void __weak update_kswapd_threads(void) { }
bool __weak current_is_nswapoutd(void) { return false; }
unsigned long __weak nswap_reclaim_page_list(struct list_head *page_list,
					     struct vm_area_struct *vma,
					     bool reclaim)
{ return 0; }
bool __weak boot_with_console(void) { return false; }
u64 __weak ktime_get_boot_ns(void) { return ktime_get_boottime_ns(); }
unsigned int __weak sysctl_fg_io_opt = 1;
unsigned long __weak oplus_silence_mode;
struct ion_wait_para __weak ionwait_para;

bool __weak is_heavy_ux_task(struct task_struct *p) { return false; }
void __weak printk_force_uart(void) { }
void __weak trace_perf_index_l(void) { }
void __weak skb_save_cb(void *skb, void *cb, unsigned int len) { }
void __weak skb_restore_cb(void *skb, void *cb, unsigned int len) { }
int __weak qdisc_enqueue_root(void *skb, void *sch, void *to_free) { return 0; }
void *__weak qdisc_dequeue_skb(void *sch) { return NULL; }
int __weak nf_register_queue_imq_handler(void *handler) { return 0; }
void __weak nf_unregister_queue_imq_handler(void) { }
void __weak pwrap_disable(void) { }
void __weak trace_ion_alloc_start(void *ptr, size_t size) { }
void __weak trace_ion_alloc_end(void *ptr, size_t size) { }
int __weak hans_check_frozen_transcation(void *sb) { return 0; }
