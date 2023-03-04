/* ********************************************************************
FILE                   : circular_queue.c

PROGRAM DESCRIPTION    : circular queue using array with variable queue data linked using SLL in queuue node, 
                         queue node is also accessed by using front node and rear node. 

AUTHOR                :  K.M. Arun Kumar alias Arunkumar Murugeswaran

KNOWN BUGS            :

NOTE                  :  

CHANGE LOGS           :

*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum systemstatus_t
{
	SUCCESS, NO_ERROR = 0, ERR_NULL_PTR, ERR_INVALID_DATA, ERR_FORMAT_INVALID , ERR_QUEUE_FULL, ERR_QUEUE_EMPTY, ERR_ENQUEUE_PROC, ERR_DEQUEUE_PROC, ERR_QUEUE_INSERT_FRONT_PROC, ERR_QUEUE_DELETE_REAR_PROC,
	ERR_QUEUE_RETRIEVE_INFO_PROC, ERR_DISP_QUEUE_PROC, ERR_INSERT_DATA_PROC, ERR_RETRIEVE_DATA_PROC
} system_status_t;

#define MAX_ELEMENTS_CIRC_QUEUE_ARR     (5)
#define MAX_DATA_SIZE                   (20)
#define MAX_NAME_CHARS                  (10)
#define EMP_MSG                                1  /* queue's node data has Employee's record */
#define NUM_MSG                                2  /* queue's node data has num data */

typedef enum queue_mainoper_t
{
  ENQUEUE_OPER = 1, DEQUEUE_OPER, RETRIEVE_QUEUE_OPER, SEND_TO_FRONT_OF_QUEUE_OPER, RECEIVE_TO_REAR_OF_QUEUE_OPER, RESET_QUEUE_OPER, EXIT_OPER
} queue_main_oper_t;

typedef enum queue_dispoper_t
{
	RETRIEVE_QUEUE_FRONT_DATA_OPER = 1, RETRIEVE_QUEUE_REAR_DATA_OPER, RETRIEVE_QUEUE_LIST_ELEMENTS_OPER, DISP_QUEUE_METADATA_OPER, RETRIEVE_NODE
} queue_disp_oper_t;

typedef enum queuereset_t
{
	DESTROY_QUEUE_ELEMENTS, RESET_QUEUE_WHOLE
} queue_reset_t;

#define TRACE_OFF                    (0)
#define TRACE_ON                     (1)

#define ERROR_OCCURED    (1)

#define CIRCULAR_QUEUE_RESET_INDEX         (-1)

#define NULL_PTR                          ((void *) 0)
#define DATA_NA                           (0)

typedef struct
{
	unsigned int msgtype;      /* differentiate kind of data in stack */
	unsigned int datalen;      /* variable data's length */
	void *databuff;             /* contains variable data */
} queue_data_t;

typedef struct queuenode
{
	void *databuff;
	struct queuenode *next_node_ptr;
} queue_node;

typedef struct
{
	queue_node *queue_array;
	queue_node *front_node; //dequeue at front_index
	queue_node *rear_node;  //enqueue at rear_index
	unsigned int max_num_elements;
	unsigned int max_datasize;
	int num_elements;
	int cur_index;
	int front_index; //dequeue at front_index
	int rear_index;  //enqueue at rear_index
} circular_queue_metahead_t;

typedef struct
{
	unsigned emp_id;              /* Employee ID, used as key */
	unsigned int name_len;       /* Total length of Emp's Name */
	void  *databuff;              /* variable Employee Name */
} emp_info;

system_status_t system_status_flag = NO_ERROR;
unsigned int trace_flag = TRACE_OFF;
circular_queue_metahead_t queue_meta_head;

int Queue_Count(const circular_queue_metahead_t *const circular_queue_metahead_ptr);
unsigned int Full_Queue(const circular_queue_metahead_t *const circular_queue_metahead_ptr);
unsigned int Empty_Queue(const circular_queue_metahead_t *const circular_queue_metahead_ptr);
unsigned int Error_or_Warning_Proc(const char *const error_trace_str, const unsigned int warn_or_error_format, const unsigned int warning_or_error_code);
unsigned int Create_Queue(circular_queue_metahead_t *const circular_queue_metahead_ptr, const unsigned int max_elements, const unsigned int max_datasize);
unsigned int Reset_Circular_Queue(const unsigned int reset_mode, circular_queue_metahead_t *const circular_queue_metahead_ptr);
unsigned int EnQueue_Proc(circular_queue_metahead_t *const circular_queue_metahead_ptr, const unsigned int data_len, void *insert_data_ptr);
unsigned int Queue_Sub_Data(unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, void *data_ptr);
unsigned int Insert_EmpData(const unsigned int insert_mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, queue_data_t *insert_emp_ptr);
unsigned int Insert_Queue_Data(const unsigned int insert_mode, circular_queue_metahead_t *const circular_queue_metahead_ptr );
unsigned int Queue_Emp_Data(unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, void *dequeue_data);
unsigned int Dequeue_Emp_Data(void *delete_data_ptr);
unsigned int Insert_NumData(const unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, queue_data_t *enqueue_num_ptr);
unsigned int Dequeue_Num_Data(void *delete_data_ptr);
unsigned int Queue_Num_Data(const unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, void **dequeue_data);
unsigned int DeQueue_Proc(circular_queue_metahead_t *const circular_queue_metahead_ptr, void **const dequeue_data_ptr);
unsigned int Queue_Insert_At_Front(circular_queue_metahead_t *const circular_queue_metahead_ptr, const unsigned int datalen,  void *insert_data_ptr);
unsigned int Queue_Delete_At_Rear(circular_queue_metahead_t *const circular_queue_metahead_ptr, void **const deleted_data_ptr);
unsigned int Queue_Disp_Info(circular_queue_metahead_t *const circular_queue_metahead_ptr, void **retrieve_data);
void Reset_Process(const unsigned int reset_mode);
unsigned int Destroy_Queue(circular_queue_metahead_t *const circular_queue_metahead_ptr );
/*------------------------------------------------------------*
FUNCTION NAME  : Queue_Count

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 1

BUGS           :
-*------------------------------------------------------------*/
int Queue_Count(const circular_queue_metahead_t *const circular_queue_metahead_ptr)
{
	return circular_queue_metahead_ptr->num_elements;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Full_Queue

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 2

BUGS           :
-*------------------------------------------------------------*/
unsigned int Full_Queue(const circular_queue_metahead_t *const circular_queue_metahead_ptr)
{
	if(circular_queue_metahead_ptr->num_elements >= circular_queue_metahead_ptr->max_num_elements)
		return ERR_QUEUE_FULL;
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Empty_Queue

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 3

BUGS           :
-*------------------------------------------------------------*/
unsigned int Empty_Queue(const circular_queue_metahead_t *const circular_queue_metahead_ptr)
{
	if(circular_queue_metahead_ptr->num_elements <= 0)
		return ERR_QUEUE_EMPTY;
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Error_or_Warning_Proc

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 4

BUGS           :
-*------------------------------------------------------------*/
unsigned int Error_or_Warning_Proc(const char *const error_trace_str, const unsigned int warn_or_error_format, const unsigned int warning_or_error_code)
{
	if(trace_flag)
	    printf("\n ERR: %s, code: %u", error_trace_str, warning_or_error_code);
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Create_Queue

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 5

BUGS           :
-*------------------------------------------------------------*/
unsigned int Create_Queue(circular_queue_metahead_t *const circular_queue_metahead_ptr, const unsigned int max_elements, const unsigned int max_datasize)
{
	if(circular_queue_metahead_ptr->queue_array == NULL)
	{
		circular_queue_metahead_ptr->queue_array = calloc(sizeof(queue_node), max_elements);
	}
	circular_queue_metahead_ptr->max_num_elements = max_elements;
	circular_queue_metahead_ptr->max_datasize =  max_datasize;
    circular_queue_metahead_ptr->num_elements = 0;
	circular_queue_metahead_ptr->front_node = NULL;
	circular_queue_metahead_ptr->rear_node = NULL;
	circular_queue_metahead_ptr->front_index = CIRCULAR_QUEUE_RESET_INDEX;
	circular_queue_metahead_ptr->rear_index = CIRCULAR_QUEUE_RESET_INDEX;
	circular_queue_metahead_ptr->cur_index = CIRCULAR_QUEUE_RESET_INDEX;
	if(trace_flag)
		printf("\n TRACE[05.01]: queue array: 0X%X, max elements: %u, max_data size: %u", \
    	circular_queue_metahead_ptr->queue_array, circular_queue_metahead_ptr->max_num_elements, circular_queue_metahead_ptr->max_datasize);
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Circular_Queue

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 6

Bugs           :
-*------------------------------------------------------------*/
unsigned int Reset_Circular_Queue(const unsigned int reset_mode, circular_queue_metahead_t *const circular_queue_metahead_ptr)
{
	unsigned int cur_arr_index;

  	if(circular_queue_metahead_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("06.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	switch(reset_mode)
	{
		case RESET_QUEUE_WHOLE:
		case DESTROY_QUEUE_ELEMENTS:
	      Destroy_Queue(circular_queue_metahead_ptr);
	      circular_queue_metahead_ptr->num_elements = 0;
	      circular_queue_metahead_ptr->front_node = NULL;
	      circular_queue_metahead_ptr->rear_node = NULL;
	      circular_queue_metahead_ptr->front_index = CIRCULAR_QUEUE_RESET_INDEX;
	      circular_queue_metahead_ptr->rear_index = CIRCULAR_QUEUE_RESET_INDEX;
	      circular_queue_metahead_ptr->cur_index = CIRCULAR_QUEUE_RESET_INDEX;
		  if(reset_mode == RESET_QUEUE_WHOLE)
		  {
			 if(circular_queue_metahead_ptr->queue_array)
	         {
		        free(circular_queue_metahead_ptr->queue_array);
			    circular_queue_metahead_ptr->queue_array = NULL;
	         }
		     circular_queue_metahead_ptr->max_num_elements = 0;
		     circular_queue_metahead_ptr->max_datasize = 0;
		  }
	    break;
        default:
            printf("\n ERR[06.02]: Invalid reset format : %u", reset_mode);
            return ERR_FORMAT_INVALID;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : EnQueue_Proc

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 7

Bugs           :
-*------------------------------------------------------------*/
unsigned int EnQueue_Proc(circular_queue_metahead_t *const circular_queue_metahead_ptr, const unsigned int data_len, void *insert_data_ptr)
{
	queue_node *temp_node;

    if(trace_flag)
		 printf("\n TRACE[07.0]");
	if(circular_queue_metahead_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("07.01", ERROR_OCCURED,system_status_flag );
		return system_status_flag;
	}
	if(insert_data_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("07.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	// if((circular_queue_metahead_ptr->front_index == 0 && circular_queue_metahead_ptr->rear_index == (circular_queue_metahead_ptr->max_num_elements - 1)) || (circular_queue_metahead_ptr->front_index == (circular_queue_metahead_ptr->rear_index + 1)))
	if(((Full_Queue(circular_queue_metahead_ptr)) != SUCCESS) || data_len > circular_queue_metahead_ptr->max_datasize)
	{
		system_status_flag = ERR_QUEUE_FULL;
		Error_or_Warning_Proc("07.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(trace_flag)
	  printf("\n TRACE[07.04]: Before Enqueue - rear: %d, rear node : 0X%X, msg_ptr: 0X%X", circular_queue_metahead_ptr->rear_index, \
	    circular_queue_metahead_ptr->rear_node, insert_data_ptr);
	//if(circular_queue_metahead_ptr->front_index == CIRCULAR_QUEUE_RESET_INDEX)
	if((Empty_Queue(circular_queue_metahead_ptr)) == ERR_QUEUE_EMPTY)
	{
		/* queue was empty */
		circular_queue_metahead_ptr->front_index = 0;
		circular_queue_metahead_ptr->rear_index = 0;
		circular_queue_metahead_ptr->rear_node = circular_queue_metahead_ptr->front_node = circular_queue_metahead_ptr->queue_array;
	}
	else
	{
		temp_node = circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->rear_index;
		if(circular_queue_metahead_ptr->rear_index == (circular_queue_metahead_ptr->max_num_elements - 1))
		{
			//rear was at last position of queue
			circular_queue_metahead_ptr->rear_index = 0;
		}
		else
		{
			++circular_queue_metahead_ptr->rear_index;
		}
		temp_node->next_node_ptr = circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->rear_index;
		circular_queue_metahead_ptr->rear_node = circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->rear_index;
	}
	circular_queue_metahead_ptr->cur_index = circular_queue_metahead_ptr->rear_index;
	(circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->rear_index)->databuff = insert_data_ptr;
	(circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->rear_index)->next_node_ptr = NULL;
	++circular_queue_metahead_ptr->num_elements;
	if(trace_flag)
	  printf("\n TRACE[07.05]: After Enqueued - rear: %d, rear node : 0X%X", circular_queue_metahead_ptr->rear_index, \
	    circular_queue_metahead_ptr->rear_node);
	return SUCCESS;
}

/*******************************************************************
 Function Name  : Queue_Sub_Data
 Description    :
 Remarks        :
 Func ID        : 8
 *******************************************************************/
 unsigned int Queue_Sub_Data(unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, void *data_ptr)
 {
    queue_data_t *ptr;
	unsigned int ret_state  ;

    if(trace_flag)
		 printf("\n TRACE[08.0]");
	if(data_ptr != NULL)
	{
		ptr = (queue_data_t *) data_ptr;
	}
	else
	{
		if(mode == RECEIVE_TO_REAR_OF_QUEUE_OPER)
		{
			mode += EXIT_OPER;
		}
	    switch(mode)
		{
			case RETRIEVE_QUEUE_FRONT_DATA_OPER + EXIT_OPER:
			case DEQUEUE_OPER:
			   	ptr = (queue_data_t *)circular_queue_metahead_ptr->front_node->databuff;
			break;
            case RETRIEVE_QUEUE_REAR_DATA_OPER + EXIT_OPER:
			case RECEIVE_TO_REAR_OF_QUEUE_OPER:
			   ptr = (queue_data_t *) circular_queue_metahead_ptr->rear_node->databuff;
            break;
            case RETRIEVE_NODE + EXIT_OPER:
            break;
            default:
                printf("\n ERR[08.01]: Invalid mode : %u", mode);
                return ERR_FORMAT_INVALID;
		}
	}
	if(ptr == NULL)
	{
		printf("\n ERR[08.02]: Null Ptr");
		return ERR_NULL_PTR;
	}
	switch(ptr->msgtype)
	{
		case EMP_MSG:
		  ret_state = Queue_Emp_Data(mode, circular_queue_metahead_ptr, ptr->databuff);
		break;
		case NUM_MSG:
		  ret_state = Queue_Num_Data(mode, circular_queue_metahead_ptr, ptr->databuff);
		break;
		default:
		    printf("\n ERR[08.03]: invalid msg type - %u", ptr->msgtype);
			free(ptr);
			return ERR_FORMAT_INVALID;
	}
	if(mode == DEQUEUE_OPER || mode == RECEIVE_TO_REAR_OF_QUEUE_OPER)
	   free(ptr);
	return SUCCESS;
 }

/*******************************************************************
   Function Name :
   Description   :
   Remarks       : assume that input data are valid
   fUNC_ID       : 9
 *******************************************************************/
unsigned int Insert_EmpData(const unsigned int insert_mode, circular_queue_metahead_t *const circular_queue_metahead_ptr,  queue_data_t *insert_emp_ptr)
{
    unsigned int ret_state ;
    emp_info *insert_emp = NULL;
    queue_data_t *queue_data_ptr;
    unsigned int queuedata_totlen;
    char *name_ptr;
	char name[MAX_NAME_CHARS];
	unsigned emp_id, name_len;

     if(trace_flag)
		 printf("\n TRACE[09.0]");
	if(insert_emp_ptr == NULL)
	{
	    printf("\n Enter Emp ID & its name: ");
        scanf(" %u %[^\n]", &emp_id, name);
       /* name's len also includes NUL character:'\0' */
        name_len = strlen(name) + 1;
       if(name_len > MAX_NAME_CHARS)
       {
	      printf("\n ERR[09.1]: Entered name %s, whose len %d exceeds tree's data limit: %u",name,name_len, MAX_NAME_CHARS);
	      return ERR_INVALID_DATA;
       }
	   if((queue_data_ptr = (queue_data_t *)calloc (1, sizeof(queue_data_t))) == NULL)
	   {
	      printf("\n ERR[09.2]: no memory to allocate");
	      return ERR_NULL_PTR;
       }
	   if((insert_emp = (emp_info *) calloc(1, sizeof(emp_info))) == NULL)
	   {
	  	 printf("\n Err[09.3]: No memory to alloc ");
	     free(queue_data_ptr);
		 return ERR_NULL_PTR;
	   }
	   if((name_ptr = (char *)calloc(1, name_len)) == NULL)
	   {
	       printf("\n Err[09.4]: No memory to alloc ");
		   free(queue_data_ptr);
		   free(insert_emp);
		   return ERR_NULL_PTR;
	   }
        queue_data_ptr->msgtype = EMP_MSG;
	    queue_data_ptr->datalen = sizeof(emp_info) + name_len - sizeof(void *);
	    queue_data_ptr->databuff = insert_emp;
	    insert_emp->emp_id = emp_id;
	    insert_emp->name_len = name_len;
	    insert_emp->databuff = name_ptr;
	    memcpy(name_ptr, name, name_len);
	}
	else
	{
		queue_data_ptr = insert_emp_ptr;
	}
	if(trace_flag)
	{
	    printf("\n TRACE[09.5]: Push Data - totstack_datalen:%u:: Msgtype:%u", queue_data_ptr->datalen, queue_data_ptr->msgtype);
	    printf("\n TRACE[09.6]: Push emp name Ptr: %#X, Emp Ptr: %#X", insert_emp, insert_emp->databuff);
	}
	switch(insert_mode)
	{
		case ENQUEUE_OPER:
	       ret_state = EnQueue_Proc(circular_queue_metahead_ptr, queue_data_ptr->datalen, queue_data_ptr);
		break;
        case SEND_TO_FRONT_OF_QUEUE_OPER:
    		ret_state = Queue_Insert_At_Front(circular_queue_metahead_ptr, queue_data_ptr->datalen, queue_data_ptr);
        break;
		default:
		  printf("\n ERR[09.7]: Invalid insert oper: %u", insert_mode);
		  return ERR_FORMAT_INVALID;
	}
	printf("\n Inserted Emp Data - emp_id: %u, emp name: %s", insert_emp->emp_id, (char *)insert_emp->databuff);
	return ret_state;
}
/*******************************************************************
 Function Name  : Insert_Queue_Data
 Description    :
 Remarks        :
 Func ID        : 10
*******************************************************************/
unsigned int Insert_Queue_Data(const unsigned int insert_mode, circular_queue_metahead_t *const circular_queue_metahead_ptr )
{
	unsigned int msgtype, ret_state;

    if(trace_flag)
		 printf("\n TRACE[10.0]");
	printf("\n Data: 1 - Emp, 2 - a number ");
	printf("\n Enter : ");
	scanf("%u", &msgtype);
	switch(msgtype)
	{
		case EMP_MSG:
		  ret_state = Insert_EmpData(insert_mode, circular_queue_metahead_ptr, NULL);
		break;
		case NUM_MSG:
		  ret_state = Insert_NumData(insert_mode, circular_queue_metahead_ptr, NULL);
		break;
		default:
		  printf("\n ERR[10.01]: Invalid enqueued data : %u", msgtype);
		  ret_state = ERR_FORMAT_INVALID;
	}
	return ret_state;
}

/*******************************************************************
   Function Name : Queue_Emp_Data
   Description   : deals Emp data
   Remarks       :
   fUNC_ID       : 11
 *******************************************************************/
 unsigned int Queue_Emp_Data(unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, void *dequeue_data)
 {
	 emp_info *emp_ptr = (emp_info *) dequeue_data;
	 unsigned int ret_state ;

     if(trace_flag)
		 printf("\n TRACE[11.0]");
	 if(dequeue_data == NULL)
	 {
		 printf("\n ERR[11.1]: Null ptr");
		 return ERR_NULL_PTR;
	 }
	 switch(mode)
	 {
		 case ENQUEUE_OPER:
		 case SEND_TO_FRONT_OF_QUEUE_OPER:
		    ret_state = Insert_EmpData(mode, circular_queue_metahead_ptr, NULL);
		 break;
		 case DEQUEUE_OPER:
		 case RECEIVE_TO_REAR_OF_QUEUE_OPER:
		     printf("\n delete Emp ID: %u, name: %s", emp_ptr->emp_id, (char *)emp_ptr->databuff);
			 ret_state = Dequeue_Emp_Data(dequeue_data);
         break;
		 case RETRIEVE_NODE + EXIT_OPER:
		 case RETRIEVE_QUEUE_FRONT_DATA_OPER + EXIT_OPER:
		 case RETRIEVE_QUEUE_REAR_DATA_OPER + EXIT_OPER:
		     printf("\n retrieve Emp ID: %u, Emp name len: %u, name: %s", emp_ptr->emp_id, emp_ptr->name_len, (char *)emp_ptr->databuff);
			 ret_state = SUCCESS;
		 break;
		 default:
             printf("\n Err[11.2]: Invalid Emp oper: %u ", mode);
			 return ERR_FORMAT_INVALID;
	 }
	 return ret_state;
 }

 /*******************************************************************
 Function Name  : Dequeue_Emp_Data(void *delete_data_ptr  )
 Description    :
 Remarks        :
 Func ID        : 12
 *******************************************************************/
 unsigned int Dequeue_Emp_Data(void *delete_data_ptr)
 {
    emp_info *del_ptr = NULL;
    char *name;
    if(trace_flag)
		 printf("\n TRACE[12.0]");

	if(delete_data_ptr == NULL)
	{
		printf("ERR[12.1]: Null Ptr");
		return ERR_NULL_PTR;
	}
    del_ptr = (emp_info *)delete_data_ptr;
	name = (char *)del_ptr->databuff;
    if(trace_flag)
	{
		printf("\n TRACE[12.2]: Deleted - Emp ID: %u, len: %u, name: %s ", del_ptr->emp_id, del_ptr->name_len, name);
	}
	free(del_ptr);
	free(name);
	return SUCCESS;
 }

/*******************************************************************
 Function Name  : Enqueue_NumData
 Description    :
 Remarks        :
 Func ID        : 13
*******************************************************************/
unsigned int Insert_NumData(const unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, queue_data_t *enqueue_num_ptr)
{
	unsigned int ret_state = SUCCESS;
    int *enqueue_num = NULL, num;
    queue_data_t *queue_data_ptr;
    unsigned int stackdata_totlen;

    if(trace_flag)
		 printf("\n TRACE[13.0]");
	if(enqueue_num_ptr == NULL)
	{
	    printf("\n Enter a num : ");
        scanf("%d", &num);
       if((queue_data_ptr = (queue_data_t *)calloc (1, sizeof(queue_data_t))) == NULL)
	   {
	      printf("\n ERR[13.1]: no memory to allocate");
	      return ERR_NULL_PTR;
       }
	   if((enqueue_num = (int *) calloc(1, sizeof(int))) == NULL)
	   {
	  	 printf("\n Err[13.2]: No memory to alloc ");
	     free(queue_data_ptr);
		 return ERR_NULL_PTR;
	   }
	    queue_data_ptr->msgtype = NUM_MSG;
	    queue_data_ptr->datalen = sizeof(int);
	    queue_data_ptr->databuff = enqueue_num;
	    *enqueue_num = num;
	}
	else
	{
		queue_data_ptr = enqueue_num_ptr;
	}
	if(trace_flag)
	{
	    printf("\n TRACE[13.3]: Insert Data - totstack_datalen:%u:: Msgtype:%u, Num Node: 0X%X", queue_data_ptr->datalen, queue_data_ptr->msgtype, enqueue_num);
	}
	switch(mode)
	{
		case ENQUEUE_OPER:
	       ret_state = EnQueue_Proc(circular_queue_metahead_ptr, queue_data_ptr->datalen, queue_data_ptr);
		break;
        case SEND_TO_FRONT_OF_QUEUE_OPER:
    		ret_state = Queue_Insert_At_Front(circular_queue_metahead_ptr, queue_data_ptr->datalen, queue_data_ptr);
        break;
	}
	printf("\n Inserted Num Data: %d", *enqueue_num);
	return ret_state;
}
/*******************************************************************
 Function Name  : Dequeue_Num_Data(void *delete_data_ptr  )
 Description    :
 Remarks        :
 Func ID        : 14
 *******************************************************************/
 unsigned int Dequeue_Num_Data(void *delete_data_ptr)
 {
    int *del_ptr = NULL;

    if(trace_flag)
		 printf("\n TRACE[14.0]");
	if(delete_data_ptr == NULL)
	{
		printf("ERR[14.1]: Null Ptr");
		return ERR_NULL_PTR;
	}
    del_ptr = (int *)delete_data_ptr;
    if(trace_flag)
	{
		printf("\n TRACE[14.2]: Deleted Num: %u", *del_ptr);
	}
	free(del_ptr);
	return SUCCESS;
 }

 /*******************************************************************
   Function Name : Queue_Num_Data
   Description   : deals num data
   Remarks       :
   fUNC_ID       : 15
 *******************************************************************/
 unsigned int Queue_Num_Data(const unsigned int mode, circular_queue_metahead_t *const circular_queue_metahead_ptr, void **dequeue_data)
 {
	 int *num_ptr = (int *) dequeue_data;
	 unsigned int ret_state = SUCCESS;
     unsigned int datalen;

     if(trace_flag)
		 printf("\n TRACE[15.0]");
	 if(num_ptr == NULL)
	 {
		 printf("\n ERR[15.1]: Null ptr");
		 return ERR_NULL_PTR;
	 }
	 switch(mode)
	 {
		 case ENQUEUE_OPER:
		 case SEND_TO_FRONT_OF_QUEUE_OPER:
		    ret_state = Insert_NumData(mode, circular_queue_metahead_ptr, NULL);
		 break;
		 case DEQUEUE_OPER:
		 case RECEIVE_TO_REAR_OF_QUEUE_OPER:
		     printf("\n delete num: %d ", *num_ptr);
		     ret_state = Dequeue_Num_Data(dequeue_data);
         break;
		 case RETRIEVE_NODE + EXIT_OPER:
		 case RETRIEVE_QUEUE_FRONT_DATA_OPER + EXIT_OPER:
		 case RETRIEVE_QUEUE_REAR_DATA_OPER + EXIT_OPER:
		     printf("\n retrieve Num: %u",  *num_ptr);
			 ret_state = SUCCESS;
		 break;
		 default:
             printf("\n Err[15.2]: Invalid oper: %u", mode);
			 ret_state = ERR_FORMAT_INVALID;
	 }
	 return ret_state;
 }
/*------------------------------------------------------------*
FUNCTION NAME  : DeQueue_Proc

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 16

Bugs           :
-*------------------------------------------------------------*/
unsigned int DeQueue_Proc(circular_queue_metahead_t *const circular_queue_metahead_ptr, void **const dequeue_data_ptr)
{
	unsigned int dequeue_datalen, num_elements;
    queue_node *dequeue_node;

    if(trace_flag)
		 printf("\n TRACE[16.0]");
	if(circular_queue_metahead_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("16.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(dequeue_data_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("16.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	//if(circular_queue_metahead_ptr->front_index == CIRCULAR_QUEUE_RESET_INDEX)
	if((Empty_Queue(circular_queue_metahead_ptr)) != SUCCESS)
	{
		system_status_flag = ERR_QUEUE_EMPTY;
		Error_or_Warning_Proc("16.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	dequeue_node = circular_queue_metahead_ptr->front_node;
	*dequeue_data_ptr = (queue_data_t *)dequeue_node->databuff;
	if(trace_flag)
	  printf("\n TRACE[16.04]: before Dequeue front: %d, dequeued node : 0X%X, data_ptr: 0X%X ", circular_queue_metahead_ptr->front_index, \
	    dequeue_node, *dequeue_data_ptr);
	num_elements = Queue_Count(circular_queue_metahead_ptr);
	//if(circular_queue_metahead_ptr->front_index == circular_queue_metahead_ptr->rear_index)
	if(num_elements == 1)
	{
		/* queue had only one element */
		circular_queue_metahead_ptr->front_index = CIRCULAR_QUEUE_RESET_INDEX;
		circular_queue_metahead_ptr->rear_index = CIRCULAR_QUEUE_RESET_INDEX;
		circular_queue_metahead_ptr->front_node = circular_queue_metahead_ptr->rear_node = NULL;
	}
	else
	{
		circular_queue_metahead_ptr->front_node = circular_queue_metahead_ptr->front_node->next_node_ptr;
		if(circular_queue_metahead_ptr->front_index == (circular_queue_metahead_ptr->max_num_elements - 1))
		{
			circular_queue_metahead_ptr->front_index = 0;
		}
		else
		{
			++circular_queue_metahead_ptr->front_index;
		}
	}
	--circular_queue_metahead_ptr->num_elements;
	if(trace_flag)
	  printf("\n TRACE[16.05]: after Dequeue front: %d, dequeued node : 0X%X, next: 0X%X ", circular_queue_metahead_ptr->front_index, \
	    circular_queue_metahead_ptr->front_node,  circular_queue_metahead_ptr->front_node->next_node_ptr);
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Queue_Insert_At_Front

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 17

Bugs           :
-*------------------------------------------------------------*/
unsigned int Queue_Insert_At_Front(circular_queue_metahead_t *const circular_queue_metahead_ptr, const unsigned int datalen,  void *insert_data_ptr)
{
	queue_node *temp_node;

    if(trace_flag)
		 printf("\n TRACE[17.0]");
	if(circular_queue_metahead_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("17.01", ERROR_OCCURED,system_status_flag );
		return system_status_flag;
	}
	if(insert_data_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("17.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	//if((circular_queue_metahead_ptr->front_index == 0 && circular_queue_metahead_ptr->rear_index == (circular_queue_metahead_ptr->max_num_elements - 1)) || (circular_queue_metahead_ptr->front_index == (circular_queue_metahead_ptr->rear_index + 1)))
	if((Full_Queue(circular_queue_metahead_ptr)) != SUCCESS || datalen > circular_queue_metahead_ptr->max_datasize)
	{
		system_status_flag = ERR_QUEUE_FULL;
		Error_or_Warning_Proc("17.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	//if(circular_queue_metahead_ptr->front_index == CIRCULAR_QUEUE_RESET_INDEX)
	if(trace_flag)
	    printf("\n TRACE[17.04]: before queue ins_front - front: %d, front node: 0X%X", \
	        circular_queue_metahead_ptr->front_index, circular_queue_metahead_ptr->front_node, insert_data_ptr);
	if((Empty_Queue(circular_queue_metahead_ptr)) == ERR_QUEUE_EMPTY)
	{
		/* queue was empty */
		circular_queue_metahead_ptr->front_index = 0;
		circular_queue_metahead_ptr->rear_index = 0;
		circular_queue_metahead_ptr->rear_node = circular_queue_metahead_ptr->front_node = circular_queue_metahead_ptr->queue_array;
        circular_queue_metahead_ptr->front_node->next_node_ptr = NULL;
	}
	else
	{
	   temp_node = circular_queue_metahead_ptr->front_node;
	   if(circular_queue_metahead_ptr->front_index == 0)
       {
           circular_queue_metahead_ptr->front_index = (circular_queue_metahead_ptr->max_num_elements - 1);
       }
	   else
	   {
	      --circular_queue_metahead_ptr->front_index;
	   }
	   circular_queue_metahead_ptr->front_node = (circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->front_index);
	   circular_queue_metahead_ptr->front_node->next_node_ptr = temp_node;
	}
	circular_queue_metahead_ptr->front_node->databuff = insert_data_ptr;
	++circular_queue_metahead_ptr->num_elements;
	if(trace_flag)
	    printf("\n TRACE[17.04]: after queue ins_front - front: %d, front node: 0X%X, msg_ptr: 0X%X", \
	        circular_queue_metahead_ptr->front_index, circular_queue_metahead_ptr->front_node, insert_data_ptr);
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Queue_Delete_At_Rear

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 18

Bugs           :
-*------------------------------------------------------------*/
unsigned int Queue_Delete_At_Rear(circular_queue_metahead_t *const circular_queue_metahead_ptr, void **const deleted_data_ptr)
{
	queue_node *delete_node, *temp_node;
	int num_elements;

    if(trace_flag)
		 printf("\n TRACE[18.0]");
	if(circular_queue_metahead_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("18.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(deleted_data_ptr == NULL_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("18.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	//if(circular_queue_metahead_ptr->front_index == CIRCULAR_QUEUE_RESET_INDEX)
	if((Empty_Queue(circular_queue_metahead_ptr)) == ERR_QUEUE_EMPTY)
	{
		system_status_flag = ERR_QUEUE_EMPTY;
		Error_or_Warning_Proc("18.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	delete_node = circular_queue_metahead_ptr->rear_node;
	*deleted_data_ptr = (queue_data_t *)delete_node->databuff;
	if(trace_flag)
	  printf("\n TRACE[18.04]: Before del_rear - rear: %d, rear node: 0X%X, msg_ptr: 0X%X", circular_queue_metahead_ptr->rear_index,
       delete_node, *deleted_data_ptr );
	//if(circular_queue_metahead_ptr->front_index == circular_queue_metahead_ptr->rear_index)
	if((Queue_Count(circular_queue_metahead_ptr)) == 1)
	{
		/* queue had only one element */
		circular_queue_metahead_ptr->front_index = CIRCULAR_QUEUE_RESET_INDEX;
		circular_queue_metahead_ptr->rear_index = CIRCULAR_QUEUE_RESET_INDEX;
		circular_queue_metahead_ptr->front_node = NULL;
		circular_queue_metahead_ptr->rear_node = NULL;
	}
	else
	{
		if(circular_queue_metahead_ptr->rear_index == 0 )
		{
			circular_queue_metahead_ptr->rear_index = (circular_queue_metahead_ptr->max_num_elements - 1);
		}
		else
		{
			--circular_queue_metahead_ptr->rear_index;
		}
		circular_queue_metahead_ptr->rear_node = circular_queue_metahead_ptr->queue_array + circular_queue_metahead_ptr->rear_index;
		circular_queue_metahead_ptr->rear_node->next_node_ptr = NULL;
	}
	--circular_queue_metahead_ptr->num_elements;
	if(trace_flag)
	   printf("\n TRACE[18.05]: After del_rear - rear: %d, rear node: 0X%X", circular_queue_metahead_ptr->rear_index,
          circular_queue_metahead_ptr->rear_node );
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Queue_Disp_Info

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 19

Bugs           :
-*------------------------------------------------------------*/
unsigned int Queue_Disp_Info(circular_queue_metahead_t *const circular_queue_metahead_ptr, void **retrieve_data)
{
	queue_data_t *retrieve_ptr;
	queue_node *retrieve_node;
	unsigned int ret_status;
	int ch;

    if(trace_flag)
		 printf("\n TRACE[19.0]");
	printf("\n Queue Disp: 1 - front, 2 - rear, 3 - items, 4 - meta data");
	printf("\n enter disp choice : ");
	scanf("%d", &ch);
	switch(ch)
	{
		case RETRIEVE_QUEUE_FRONT_DATA_OPER:
		    retrieve_node = (queue_node *) circular_queue_metahead_ptr->front_node;
			*retrieve_data = retrieve_node->databuff;
		    if((Queue_Sub_Data(ch + EXIT_OPER, circular_queue_metahead_ptr, (void *) *retrieve_data)) != SUCCESS)
			{
				printf("\n ERR[19.01]: Retrieve front");
				return ERR_DISP_QUEUE_PROC;
			}
		break;
		case RETRIEVE_QUEUE_REAR_DATA_OPER:
	    	retrieve_node = (queue_node *) circular_queue_metahead_ptr->rear_node;
			*retrieve_data = retrieve_node->databuff;
		    if((Queue_Sub_Data(ch + EXIT_OPER, circular_queue_metahead_ptr, (void *) *retrieve_data)) != SUCCESS)
			{
				printf("\n ERR[19.02]: Retrieve rear ");
				return ERR_DISP_QUEUE_PROC;
			}
		break;
		case RETRIEVE_QUEUE_LIST_ELEMENTS_OPER:
		    retrieve_node = circular_queue_metahead_ptr->front_node;
			*retrieve_data = retrieve_node->databuff;
			while(retrieve_node)
			{
				*retrieve_data = retrieve_node->databuff;
				if((Queue_Sub_Data(RETRIEVE_NODE + EXIT_OPER, circular_queue_metahead_ptr, (void *) *retrieve_data)) != SUCCESS)
			    {
				    printf("\n ERR[19.03]: Retrieve list ");
				    return ERR_DISP_QUEUE_PROC;
			    }
				retrieve_node = retrieve_node->next_node_ptr;
			}
		break;
		case DISP_QUEUE_METADATA_OPER:
		  printf("\n Array: 0X%X, max_elements: %u, num_elements: %u, max_datalen:  %u", circular_queue_metahead_ptr->queue_array, \
		    circular_queue_metahead_ptr->max_num_elements, circular_queue_metahead_ptr->num_elements, circular_queue_metahead_ptr->max_datasize);
		  printf("\n Front Index: %d, Front node: 0X%0X, rear index: %d, rear node: 0X%X", circular_queue_metahead_ptr->front_index, \
		    circular_queue_metahead_ptr->front_node, circular_queue_metahead_ptr->rear_index, circular_queue_metahead_ptr->rear_node);
		break;
		default:
		  system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("19.04", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Process

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 20

Bugs           :
-*------------------------------------------------------------*/
void Reset_Process(const unsigned int reset_mode)
{
	if(trace_flag)
		 printf("\n TRACE[20.0]");
   Reset_Circular_Queue(reset_mode, &queue_meta_head);
}
/*******************************************************************
  Function Name  : Destroy_Queue
  Description    : makes the queue empty
  Remarks        :
  Func ID        : 21
  *******************************************************************/
   unsigned int Destroy_Queue(circular_queue_metahead_t *const circular_queue_metahead_ptr )
   {
 	  queue_node *cur_node = NULL;
      queue_data_t *del_buff;
      int node_count = 0, cur_index;

      if(trace_flag)
		 printf("\n TRACE[21.0]");
 	  if((Empty_Queue(circular_queue_metahead_ptr)) == ERR_QUEUE_EMPTY)
	  {
		   // printf("\n ERR[21.1]: S's Head Ptr: %#X, is already empty to destroy",circular_queue_metahead_ptr);
			return ERR_QUEUE_EMPTY;
	  }
 	  cur_node  = circular_queue_metahead_ptr->front_node;
	  while(cur_node)
	  {
		  del_buff = (queue_data_t *)cur_node->databuff;
		  if((Queue_Sub_Data(DEQUEUE_OPER, circular_queue_metahead_ptr, (void *) del_buff)) != SUCCESS)
		  {
			    printf("\n ERR[21.02]: destroy list ");
				return ERR_DEQUEUE_PROC;
		  }
		  cur_node = cur_node->next_node_ptr;
	  }
      return SUCCESS;
  }
/*------------------------------------------------------------*
FUNCTION NAME  : main

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 22

Bugs           :
-*------------------------------------------------------------*/
int main()
{
	queue_data_t *temp_data;
	unsigned int datalen, ret_status;;
    int ch;

  	Reset_Process(RESET_QUEUE_WHOLE);
    Create_Queue(&queue_meta_head, MAX_ELEMENTS_CIRC_QUEUE_ARR, MAX_DATA_SIZE);
    printf("\n Circular Queue operations\n");
    while(1)
    {
    	printf("\n 1 - ins_rear, 2 - del_front, 3 - disp, 4 - ins_front, 5 - del_rear, 6 - reset, 7 - exit\n");
        printf("Enter your choice : ");
        scanf("%d",&ch);
        switch(ch)
        {
        case ENQUEUE_OPER:
		case SEND_TO_FRONT_OF_QUEUE_OPER:
            if((ret_status = Insert_Queue_Data(ch, &queue_meta_head)) != SUCCESS)
			      continue;
        break;
        case DEQUEUE_OPER:
		    if((DeQueue_Proc(&queue_meta_head,(void **)&temp_data)) != SUCCESS )
		        continue;
			 if((Queue_Sub_Data(ch, &queue_meta_head, (void *)temp_data)) != SUCCESS)
				 continue;
        break;
		case RECEIVE_TO_REAR_OF_QUEUE_OPER:
		   if((Queue_Delete_At_Rear(&queue_meta_head, (void **)&temp_data)) != SUCCESS)
		      continue;
		   if((Queue_Sub_Data(ch, &queue_meta_head, (void *)temp_data)) != SUCCESS)
				 continue;
        break;
        case RETRIEVE_QUEUE_OPER:
           if((ret_status = Queue_Disp_Info(&queue_meta_head, (void **)&temp_data)) != SUCCESS)
		      continue;
		break;
        case RESET_QUEUE_OPER:
        	Reset_Process(DESTROY_QUEUE_ELEMENTS);
        break;
        case EXIT_OPER:
		   Reset_Process(RESET_QUEUE_WHOLE);
		   return SUCCESS;
		break;
        default:
		    printf("\n ERR[22.1]: Invalid option: %d", ch);
        }
    }
    return 0;
}


