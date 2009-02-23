#ifndef __TIUHAL_H
#define __TIUHAL_H

typedef struct {
	char field0;
	char field1;
	char notused0;
	char notused1;
	int  field2;
	char  field3;
	char notused2;
	char notused3;
	char notused4;
	int  field4;
} tiuhw_device;



int hwu_get_tiuhw_if(void);
int tiuhw_init_hal(tiuhw_device *a, int b);

#endif // __TIUHAL_H
