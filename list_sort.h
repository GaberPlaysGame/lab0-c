/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_LIST_SORT_H
#define _LINUX_LIST_SORT_H

#include <linux/types.h>

struct list_head;

typedef int (*list_cmp_func_t)(const struct list_head *,
                               const struct list_head *);

void list_sort(void *priv, struct list_head *head, list_cmp_func_t cmp);

void q_list_sort(struct list_head *head);
#endif