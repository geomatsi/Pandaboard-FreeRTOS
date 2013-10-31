#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#define MAILBOX_IRQ 34

#define HOST_TO_M3_MBX       0 /* Rx on M3 from Host */
#define M3_TO_HOST_MBX       1 /* Tx to Host from M3 */

void enable_mailbox_irq(void);
unsigned int mailbox_read(void);
void mailbox_send(unsigned int target, unsigned int msg);
void mailbox_kick(unsigned int target);

#endif /* __MAILBOX_H__ */
