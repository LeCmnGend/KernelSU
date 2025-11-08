#ifndef KSU_FILE_PROXY_H
#define KSU_FILE_PROXY_H

#include <linux/file.h>
#include <linux/fs.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
typedef unsigned int __poll_t;
#endif

struct ksu_file_proxy {
	struct file* orig;
	struct file_operations ops;
};

struct ksu_file_proxy* ksu_create_file_proxy(struct file* fp);
void ksu_delete_file_proxy(struct ksu_file_proxy* data);

#endif // KSU_FILE_PROXY_H
