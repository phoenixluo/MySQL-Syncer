
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
static char *rs_binlog_parse_decimal(char *p, u_char *cm, uint32_t ml,
    uint32_t fl, uint32_t *dl);
static uint32_t rs_binlog_decimal_binsize(uint32_t precision, uint32_t scale);

/* meta_len, fixed_len, handler */
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
    { 2, 0, rs_binlog_parse_decimal }, /* 246 : NEWDEICMAL */
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

static const uint32_t dig2bytes[10] = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 4 };

static char *rs_binlog_parse_def(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    *dl = fl;

    rs_log_debug(RS_DEBUG_BINLOG, 0, "def fixed_len %u", *dl);

    return p;    
}

static char *rs_binlog_parse_varchar(char *p, u_char *cm, uint32_t ml, 
        uint32_t fl, uint32_t *dl) 
{
    uint32_t pack_len, max;

    max = 0;
    pack_len = 2;

    rs_memcpy(&max, cm, ml);

    if(max < 256) {
       pack_len = 1; 
    }

    rs_memcpy(dl, p, pack_len);

    rs_log_debug(RS_DEBUG_BINLOG, 0, "varchar pack %u max %u, len %u", 
            pack_len, max, *dl);

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

    rs_log_debug(RS_DEBUG_BINLOG, 0, "bit pack %u len %u", len, *dl);

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

    rs_log_debug(RS_DEBUG_BINLOG, 0, "blob pack %u len %u", pack_len, *dl);

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

    rs_log_debug(RS_DEBUG_BINLOG, 0, "varstring pack %u len %u", 
            pack_len, *dl);

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

    rs_log_debug(RS_DEBUG_BINLOG, 0, "string pack %u max %u len %u type %u", 
            pack_len, max_len, *dl, type);

    return p + pack_len;
}

static char *rs_binlog_parse_decimal(char *p, u_char *cm, uint32_t ml,
    uint32_t fl, uint32_t *dl)
{
    uint32_t    pack_len, precision, decimal;

    pack_len = 0;
    decimal = *cm++;
    precision = *cm;

    rs_log_debug(RS_DEBUG_BINLOG, 0, "decimal decimal %u precision %u", 
            decimal, precision);

    pack_len = rs_binlog_decimal_binsize(decimal, precision);

    rs_log_debug(RS_DEBUG_BINLOG, 0, "decimal pack_len %u", pack_len);

    *dl = pack_len;

    return p;
}

static uint32_t rs_binlog_decimal_binsize(uint32_t decimal, uint32_t precision)
{
    uint32_t intg, intg0, frac0, intg0x, frac0x;

        intg = decimal - precision;
        intg0 = intg / 9;
        frac0 = precision / 9;
        intg0x = intg - intg0 * 9;
        frac0x = precision - frac0 * 9;

        return intg0 * sizeof(int32_t) + dig2bytes[intg0x] + 
            frac0 * sizeof(int32_t) + dig2bytes[frac0x];
}


int rs_dm_binlog_row(rs_slave_info_t *si, void *data, uint32_t len, char type, 
        rs_binlog_obj_func before_parse_handle,        
        rs_binlog_obj_func after_parse_handle,        
        rs_binlog_dm_func write_handle, 
        rs_binlog_dm_func before_update_handle,
        rs_binlog_dm_func update_handle,
        rs_binlog_dm_func delete_handle,
        rs_binlog_dm_func free_handle,
        rs_dm_pos_alloc_t *pas_arr, void *obj)
{
    char                        *p, *ubp, *dp;
    u_char                      *ctp, *cmp;
    uint32_t                    i, j, un, nn, before, ml, cn, cl, dl, t;
    rs_binlog_dm_func           handle;
    rs_binlog_column_meta_t     *meta;
    rs_dm_pos_alloc_t           pas;
    rs_pstr_t                   *pstr;

    p = data;
    before = 0;
    dl = 0;
    t = 0;
    ubp = NULL;
    handle = NULL;
    ctp = NULL;
    cmp = NULL;
    meta = NULL;
    pstr = NULL;

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

        /* before parse struct handle */
        if(before_parse_handle != NULL) {
            before_parse_handle(obj);
        }

        /* parse every column */
        for(i = 0; i < cn; i++) {

            dl = 0;

            t = *ctp;
            if(t >= 256) {
                rs_log_error(RS_LOG_ERR, 0, "unknown col type %u", t); 
                return RS_ERR;
            }

            rs_log_debug(RS_DEBUG_BINLOG, 0, "col idx %u type %u", i, t);

            /* not used */
            if(!((ubp[i / 8] >> (i % 8))  & 0x01)) {
                rs_log_debug(RS_DEBUG_BINLOG, 0, "col idx %u nouse", i);
                goto next_col;
            }

            /* not null */
            if(((null_bits[j / 8] >> (j % 8)) & 0x01)) {
                rs_log_debug(RS_DEBUG_BINLOG, 0, "col idx %u null", i);
                goto next_col; 
            }

            meta = (rs_binlog_column_meta_t *) &(rs_column_meta[t]);

            if(meta->parse_handle == NULL) {
                rs_log_error(RS_LOG_ERR, 0, "not support mysql type parse "
                        "handle, please contact the lazy author!"); 
                return RS_ERR;
            }

            p = meta->parse_handle(p, cmp, meta->meta_len, 
                    meta->fixed_len, (uint32_t *) &dl);

            /* parse */
            pas = pas_arr[i];
            if(pas.pos == -1) {
                goto skip_col;
            }

            dp = (char *) obj + pas.pos;

            if(pas.alloc == RS_DM_DATA_STACK) {
                if(pas.type == RS_DM_TYPE_HEX) {
                    dp = rs_cpymem(dp, &dl, 4);        
                }
                rs_memcpy(dp, p, dl);
            } else if(pas.alloc == RS_DM_DATA_POOL) {
                /* rs_pstr_t */
                pstr = (rs_pstr_t *) dp;

                pstr->id = rs_palloc_id(si->dpool, dl);
                pstr->len = dl;
                pstr->data = rs_palloc(si->dpool, dl, pstr->id);

                if(pstr->data == NULL) {
                    return RS_ERR;
                }

                rs_memcpy(pstr->data, p, dl);
            } else {
                rs_log_error(RS_LOG_ERR, 0, "unknown alloc %d", pas.alloc); 
                return RS_ERR;
            }

skip_col:
            p += dl;
next_col:
            /* not null */
            j++;
            /* next column type */
            ctp++;
            /* next meta info */
            cmp += meta->meta_len;
        }

        /* after parse struct handle */
        if(after_parse_handle != NULL) {
            after_parse_handle(obj);
        }

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

        /* dm handle */
        if(handle != NULL && handle(si, obj) == RS_ERR) {
            return RS_ERR;
        }

        /* free resource handle */
        if(free_handle != NULL && free_handle(si, obj) == RS_ERR) {
            return RS_ERR;
        }
    }

    return RS_OK;
}
