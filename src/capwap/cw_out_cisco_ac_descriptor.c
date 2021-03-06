#include "log.h"
#include "conn.h"

#include "mbag.h"
#include "capwap_items.h"
#include "capwap.h"


int cw_out_cisco_ac_descriptor(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{

	uint8_t *d = dst+4;
	struct mbag_item * i;
	i = mbag_get(conn->local,CW_ITEM_AC_STATUS);
	
	if (!i) {
		cw_log(LOG_ERR,"Can't send AC Descriptor, no AC Status Item found");
		return 0; 
	}

	d+=cw_put_ac_status(d ,(struct cw_ac_status*)(i->data));

	
//	i = mbag_get(conn->local,CW_ITEM_AC_SOFTWARE_VERSION);
	i = mbag_get(conn->incomming,CW_ITEM_WTP_SOFTWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,1,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't set Cisco Software Version in AC descriptor, No value defined.");
	}

	i = mbag_get(conn->local,CW_ITEM_AC_HARDWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,0,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't set Cisco Hardware Version in AC descriptor, No value defined.");
	}




	int len = d-dst-4;
	
	return len + cw_put_elem_hdr(dst,a->elem_id,len);

}



