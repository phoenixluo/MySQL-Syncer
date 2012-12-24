
#include <rs_config.h>
#include <rs_core.h>
#include <rs_slave.h>

static char *rs_binlog_parse_def(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl);
static char *rs_binlog_parse_varchar(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl);
static char *rs_binlog_parse_blob(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl);
static char *rs_binlog_parse_varstring(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl);
static char *rs_binlog_parse_bit(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl);
static char *rs_binlog_parse_string(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl);

static rs_binlog_column_meta_t rs_column_meta[] = {
    { 0, 0, NULL },                             /* 0 : DECIMAL */
    { 0, 1, rs_binlog_parse_def },              /* 1 : TINYINT */
    { 0, 2, rs_binlog_parse_def },              /* 2 : SHORT */
    { 0, 4, rs_binlog_parse_def },              /* 3 : LONG */
    { 1, 4, rs_binlog_parse_def },              /* 4 : FLOAT */
    { 1, 8, rs_binlog_parse_def },              /* 5 : DOUBLE */
    { 0, 0, NULL },                             /* 6 : NULL */
    { 0, 4, rs_binlog_parse_def },              /* 7 : TIMESTAMP */
    { 0, 8, rs_binlog_parse_def },              /* 8 : LONGLONG */
    { 0, 3, rs_binlog_parse_def },              /* 9 : INT24 */
    { 0, 3, rs_binlog_parse_def },              /* 10: DATE */
    { 0, 3, rs_binlog_parse_def },              /* 11: TIME */
    { 0, 8, rs_binlog_parse_def },              /* 12: DATETIME */
    { 0 , 1 , rs_binlog_parse_def },            /* 13: YEAR */
    { 0, 0, NULL },                             /* 14 : - */
    { 2, 0, rs_binlog_parse_varchar },          /* 15: VARCHAR */
    { 2, 0, rs_binlog_parse_bit },              /* 16: BIT */
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 0, 0, NULL },
    { 1, 0, rs_binlog_parse_blob },
    { 2, 0, rs_binlog_parse_varstring },
    { 2, 0, rs_binlog_parse_string },
    { 1, 0, rs_binlog_parse_blob }
};

static char *rs_binlog_parse_def(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    *dl = fl;
    return p;    
}

static char *rs_binlog_parse_varchar(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    uint32_t pack_len, max_len;

    max_len = 0;

    rs_memcpy(&max_len, cm, ml);

    if(max_len < 256) {
       pack_len = 1; 
    } else {
        pack_len = 2;
    }

    rs_log_slave(0, "parse_varchar max_len : %u, pack_len : %u", max_len, 
            pack_len);

    rs_memcpy(dl, p, pack_len);

    return p + pack_len;    
}

/* not test */
static char *rs_binlog_parse_bit(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    uint32_t len;
    len = 0;

    rs_memcpy(&len, cm, 1);

    *dl = len / 8;

    return p;    
}

/* not test */
static char *rs_binlog_parse_blob(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    uint32_t pack_len;

    pack_len = 0;

    rs_memcpy(&pack_len, cm, ml);
    rs_memcpy(dl, p, pack_len);

    return p + pack_len;    
}

/* not test */
static char *rs_binlog_parse_varstring(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    uint32_t pack_len;

    pack_len = 0;

    rs_memcpy(&pack_len, cm + 1, 1);
    rs_memcpy(dl, p, pack_len);

    rs_log_slave(0, "pack len : %u", pack_len);

    return p + pack_len;    
}

static char *rs_binlog_parse_string(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    uint32_t pack_len, max_len, type;

    max_len = 0;
    type = 0;

    /* MYSQL BUG : 37426 */
    if((*cm & 0x30) != 0x30) {
        max_len =  *(cm + 1) | (((*cm & 0x30) ^ 0x30) << 4);
        type = *cm | 0x30;
    } else {
        rs_memcpy(&max_len, cm + 1, 1);
    }

    if(max_len < 256) {
       pack_len = 1; 
    } else {
        pack_len = 2;
    }

    rs_memcpy(dl, p, pack_len);

    rs_log_slave(0, "parse_string max_len : %u, pack_len : %u, type : %u", 
            max_len, pack_len, type);

    return p + pack_len;
}


int rs_dml_binlog_row(rs_slave_info_t *si, void *data, uint32_t len, char type, 
        rs_binlog_obj_func before_parse_handle,        
        rs_binlog_obj_func after_parse_handle,        
        rs_binlog_dml_func write_handle, 
        rs_binlog_dml_func before_update_handle,
        rs_binlog_dml_func update_handle,
        rs_binlog_dml_func delete_handle,
        int32_t *offset_arr, void *obj)
{
    char                        *p, *ubp;
    u_char                      *ctp, *cmp;
    uint32_t                    i, j, un, nn, before, ml, cn, cl, cmdn, dl, t;
    int                         cmd;
    rs_binlog_dml_func          handle;
    rs_binlog_column_meta_t     *meta;

    p = data;
    before = 0;
    cmdn = 0;
    dl = 0;
    t = 0;
    ubp = NULL;
    handle = NULL;
    ctp = NULL;
    cmp = NULL;
    meta = NULL;

    /* get column number */
    rs_memcpy(&cn, p, 4);
    p += 4;

    /* get column type */
    u_char ct[cn];
    rs_memcpy(ct, p, cn);
    p += cn;

    /* get column meta length */
    rs_memcpy(&ml, p, 4);
    p += 4;

    u_char cm[ml];
    /* get column meta */
    rs_memcpy(cm, p, ml);
    p += ml;

    /* skip table id and reserved id */
    p += 8;

    /* get column number */
    cn = (uint32_t) rs_parse_packed_integer(p, &cl);
    p += cl;

    /* get used bits */
    un = (cn + 7) / 8;

    char use_bits[un], use_bits_after[un];

    rs_memcpy(use_bits, p, un);
    p += un;

    if(type == RS_UPDATE_ROWS_EVENT) {
        rs_memcpy(use_bits_after, p, un);
        p += un;
    }

    /* get column value */
    while(p < (char *) data + len) {

        j = 0;    
        cmd = 0;
        nn = (un * 8 + 7) / 8;
        char null_bits[nn];
        ctp = ct;
        cmp = cm;

        rs_memcpy(null_bits, p, nn);
        p += nn;


        if((type != RS_UPDATE_ROWS_EVENT || before == 0)) {
            ubp = use_bits;
        } else {
            ubp = use_bits_after;
        }

        if(before_parse_handle != NULL) {
            before_parse_handle(obj);
        }

        /* parse every column */
        for(i = 0; i < cn; i++) {

            dl = 0;

            t = *ctp;
            if(t >= 256) {
                rs_log_err(0, "unknow mysql binlog type %u", t); 
                return RS_ERR;
            }

            meta = (rs_binlog_column_meta_t *) &(rs_column_meta[t]);

            if(meta->parse_handle == NULL) {
                rs_log_err(0, "not support mysql type parse handle, please "
                        "contact the author"); 
                return RS_ERR;
            }

            p = meta->parse_handle(p, cmp, meta->meta_len, 
                    meta->fixed_len, (uint32_t *) &dl);

            rs_log_slave(0, "column type : %u, data len : %u", t, dl);

            /* used */
            if((ubp[i / 8] >> (i % 8))  & 0x01) {

                /* not null */
                if(!((null_bits[j / 8] >> (j % 8)) & 0x01)) {
                    /* parse */
                    if(offset_arr[i] > -1) {
                        rs_memcpy((char *) obj + offset_arr[i], p, dl);
                    }
                    p += dl;
                }

                j++;
            } else {
                rs_log_slave(0, "column index : %u not used", i);
            }
            
            /* next column type */
            ctp++;
            /* next meta info */
            cmp += meta->meta_len;
        }

        if(after_parse_handle != NULL) {
            after_parse_handle(obj);
        }

        /* append redis cmd */
        if(type == RS_WRITE_ROWS_EVENT) {
            handle = write_handle;
        } else if(type == RS_UPDATE_ROWS_EVENT) {
            if(before++ == 1) {
                handle = update_handle;
                before = 0;
            } else {
                handle = before_update_handle;
            }
        } else if(type == RS_DELETE_ROWS_EVENT) {
            handle = delete_handle;
        } else {
            return RS_ERR;
        }

        if(handle == NULL) {
            continue;
        }

        if((cmd = handle(si, obj)) == RS_ERR) {
            return RS_ERR;
        }

        cmdn += (uint32_t) cmd;
    }

    si->cmdn += cmdn;

    return RS_OK;
}
