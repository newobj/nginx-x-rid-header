#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>
#include <ngx_http_variables.h>

#if (NGX_FREEBSD)
#error FreeBSD is not supported yet, sorry.
#elif (NGX_LINUX)
#include <uuid.h>      
#elif (NGX_SOLARIS)
#error Solaris is not supported yet, sorry.
#elif (NGX_DARWIN)
#include <uuid/uuid.h>      
#endif

// TODO:
//
// * make the name of the variable configurable

ngx_int_t ngx_x_rid_header_get_variable(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data) {
  u_char *p;     

  p = ngx_pnalloc(r->pool, 37);
  if (p == NULL) {
      return NGX_ERROR;
  }       
      
#if (NGX_FREEBSD)
#error FreeBSD is not supported yet, sorry.
#elif (NGX_LINUX)
  uuid_t* uuid;
  if ( uuid_create(&uuid) ) {
    return -1;
  }
  if ( uuid_make(uuid, UUID_MAKE_V4) ) {
    uuid_destroy(uuid);
    return -1;
  }
  size_t data_len = 37;
  if ( uuid_export(uuid, UUID_FMT_STR, &p, &data_len) ) {
    uuid_destroy(uuid);
    return -1;
  }
  uuid_destroy(uuid);
#elif (NGX_SOLARIS)
#error Solaris is not supported yet, sorry.
#elif (NGX_DARWIN)
  uuid_t uuid;
  uuid_generate(uuid);       
  uuid_unparse_lower(uuid, (char*)p);
#endif

  v->len = 36;
  v->valid = 1;
  v->no_cacheable = 0;
  v->not_found = 0;
  v->data = p;

  return NGX_OK;
}   
                                  
static ngx_str_t  ngx_x_rid_header_variable_name = ngx_string("request_id");

static ngx_int_t ngx_x_rid_header_add_variables(ngx_conf_t *cf)
{
  ngx_http_variable_t* var = ngx_http_add_variable(cf, &ngx_x_rid_header_variable_name, NGX_HTTP_VAR_NOHASH);
  if (var == NULL) {
      return NGX_ERROR;
  }
  var->get_handler = ngx_x_rid_header_get_variable;
  return NGX_OK;
}
               
static ngx_http_module_t  ngx_x_rid_header_module_ctx = {
  ngx_x_rid_header_add_variables,     /* preconfiguration */
  NULL,                               /* postconfiguration */

  NULL,        /* create main configuration */
  NULL,        /* init main configuration */
            
  NULL,        /* create server configuration */
  NULL,        /* merge server configuration */
            
  NULL,        /* create location configuration */
  NULL         /* merge location configuration */
};                        

static ngx_command_t  ngx_x_rid_header_module_commands[] = {
  ngx_null_command
};
                      
ngx_module_t  ngx_x_rid_header_module = {
  NGX_MODULE_V1,
  &ngx_x_rid_header_module_ctx,      /* module context */
  ngx_x_rid_header_module_commands,  /* module directives */
  NGX_HTTP_MODULE,                   /* module type */
  NULL,                              /* init master */              
  NULL,                              /* init module */              
  NULL,                              /* init process */             
  NULL,                              /* init thread */              
  NULL,                              /* exit thread */              
  NULL,                              /* exit process */             
  NULL,                              /* exit master */              
  NGX_MODULE_V1_PADDING   
};

