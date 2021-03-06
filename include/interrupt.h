#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

void nvic_enable_irq(unsigned int irq);
void nvic_clear_irq(unsigned int irq);
void nvic_force_irq(unsigned int irq);

#endif /* __INTERRUPT_H__ */
