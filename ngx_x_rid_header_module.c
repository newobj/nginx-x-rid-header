#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>
#include <ngx_http_variables.h>
#include <uuid/uuid.h>      

// TODO:
//
// * make the name of the variable configurable
// * is there a more portable way of getting uuid in here?

ngx_int_t ngx_x_rid_header_get_variable(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data) {
  u_char *p;     

  p = ngx_pnalloc(r->pool, 36);
  if (p == NULL) {
      return NGX_ERROR;
  }       
                
  uuid_t uuid;
  uuid_generate(uuid);       
  uuid_unparse_lower(uuid, (char*)p);

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

char* ngx_x_rid_header_conf(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {                                                                          
  return NGX_CONF_OK;
}

static ngx_command_t  ngx_x_rid_header_module_commands[] = {
  { ngx_string("request_id_header"),
    NGX_HTTP_MAIN_CONF|NGX_CONF_NOARGS,
    ngx_x_rid_header_conf,
    NGX_HTTP_MAIN_CONF_OFFSET,
    0,
    NULL },
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
               