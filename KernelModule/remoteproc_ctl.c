#include <linux/init.h>
#include <linux/module.h>
#include <linux/remoteproc.h>

/* */

#define DEFAULT_RCPU "ipu_c0"

/* */

static char *rcpu_name = NULL;
static struct rproc *rcpu = NULL;

/* */

module_param(rcpu_name, charp, 0444);
MODULE_PARM_DESC(rcpu_name, "Remote cpu name");

/* */

static int rproc_init(void)
{
	if (!rcpu_name)
		rcpu_name = DEFAULT_RCPU;

	rcpu = rproc_get_by_name(rcpu_name);

	if (!rcpu) {
		printk(KERN_ERR "Could not get remote cpu by name %s\n", rcpu_name);
	}

	/* ok: remote cpu should be up and running */
	printk(KERN_INFO "Remote cpu should be up and running\n");

	return 0;
}

static void rproc_exit(void)
{
	if (rcpu) {
		rproc_put(rcpu);
		printk(KERN_INFO "Shut down remote cpu\n");
	}
}

module_init(rproc_init);
module_exit(rproc_exit);

MODULE_LICENSE("Dual BSD/GPL");
