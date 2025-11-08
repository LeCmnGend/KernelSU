#ifndef __KSU_H_SECCOMP_CACHE
#define __KSU_H_SECCOMP_CACHE

#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cred.h>

// Checks for UH, KDP and RKP
#ifdef SAMSUNG_UH_DRIVER_EXIST
#if defined(CONFIG_UH) || defined(CONFIG_KDP) || defined(CONFIG_RKP)
#error "CONFIG_UH, CONFIG_KDP and CONFIG_RKP is enabled! Please disable or remove it before compile a kernel with KernelSU!"
#endif
#endif

extern long ksu_strncpy_from_user_nofault(char *dst,
                      const void __user *unsafe_addr,
                      long count);
extern long ksu_strncpy_from_user_retry(char *dst,
                      const void __user *unsafe_addr,
                      long count);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0) ||    \
    defined(CONFIG_IS_HW_HISI) ||    \
    defined(CONFIG_KSU_ALLOWLIST_WORKAROUND)
extern struct key *init_session_keyring;
#endif

extern long ksu_copy_from_user_nofault(void *dst, const void __user *src, size_t size);

/*
 * ksu_copy_from_user_retry
 * try nofault copy first, if it fails, try with plain
 * paramters are the same as copy_from_user
 * 0 = success
 */
static long ksu_copy_from_user_retry(void *to, 
        const void __user *from, unsigned long count)
{
    long ret = ksu_copy_from_user_nofault(to, from, count);
    if (likely(!ret))
        return ret;

    // we faulted! fallback to slow path
    return copy_from_user(to, from, count);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define ksu_access_ok(addr, size)    access_ok(addr, size)
#else
#define ksu_access_ok(addr, size)    access_ok(VERIFY_READ, addr, size)
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 2) // Android backport this feature in 5.10.2
extern void ksu_seccomp_clear_cache(struct seccomp_filter *filter, int nr);
extern void ksu_seccomp_allow_cache(struct seccomp_filter *filter, int nr);
#endif

#endif