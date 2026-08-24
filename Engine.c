engine{
    #include <limits.h>
    #define MAX 300

    typedef struct instruction{
        compiler * c ;
        int op_code ;
        int p1 ; 
        int p2 ;
        int p3 ;
        void * p4  ;
    }
                    
    /* typedef struct column_def {
        char * name;
        data_type type;
        bool is_primary_key;
    } column_def;

    typedef struct sql_master {
        char * table_name;
        int table_num ; 
        int page_num;
        column_def * columns;   
        int num_columns;
        int num_rows ; 
    } sql_master; 
     
    typedef struct sql_master_list {
        sql_master * entries[300];  
        int num_tables;
    } sql_master_list;

    typedef struct dab {
        pager * pager;             
        sql_master_list * master;   
        char * filename;           
    } dab;
    dab * db */


    // what you need to do is just take up the stuff annd make the bytecode of it simple need to like get the the hash cursor and then the register where the stuff is prsemt and then one of the register which liek holds all the stuff in it the primary keys and all the info liek and then and then push it to the bytecode and then it process it and boom ohh god i reallty didnt wanted it to get extended but its alright i guess 

    typedef struct compiler{
        type * typ ; 
        int count ; 
        int capacity ; 
        int register_counter ; 
        int sorter_cursor ; 
        int register_start ; 
        tables_list * tl ; 
        int cursor_num ; 
        select_info *select ; 
    }

    typedef struct plan {
        bool where ; 
        bool orderby ; 
        bool groupby ; 
        bool case ; 
        bool join ; 
        bool from ; 
        bool select ; 
        bool case ; 
        bool having ; 
    }

    typedef struct comp{
        plan pl ; 
        int num_of_stuff ; 
        int current_num ; 
    }

    void emit(compiler * c , int op_code , int p1 , int p2 , int p3 , void * p4  ){
        if (c->count == c->capacity ){
            c->capactiy  = c->capacity * 2 ; 
            c->typ = realloc(c->typ , c->capacity * sizeof(type)) ; 
        }
        c->typ[c->count].operand =  op_code ; 
        c->typ[c->count].p1 = p1 ; 
        c->typ[c->count].p2 = p2 ; 
        c->typ[c->count].p3 = p3 ; 
        c->typ[c->count].p4 = p4 ; 
        c->count++ ; 

    }

    enum column_types{ 
        expression = 1 , 
        column_name , 
        aggregate_func  , 
        normal 
    }


    typedef struct select_select_info {
        column_types col_tp ;
        char *col_name ; 
        char *extra_col ; 
        char * operator ; 
        select_select_info *left ; 
        select_select_info * right ; 
        char * as ; 
        float *float_val ; 
        unsigned char * blob ; 
        int * num_value ; 
        char * char_value ; 
        int acc_reg ; 
    }

    typedef struct select_join_info{
        char * table_or_col_name ; 
        float *float_val ; 
        unsigned char * blob ; 
        int * num_value ; 
        char * char_value ; 
        char * operator ; 
        char * as  ; 
        char *extra_table_or_col_name ; 
        select_join_info *left ; 
        select_join_info * right ; 
        select_select_info * on ; 
    }

    typedef struct select_from_info{
        char * table_name ; 
        select_join_info * join ; 
        char * as  ; 
    }

    typedef struct extra_info_ob{
        char * nulls   ; 
        char * order   ; 
    }
    typedef struct select_ob_info{
        char *ob_name ; 
        char *extra_ob_name ; 
        char *dir ; 
        extra_info_ob * ex ; 
        char * as  ; 
        char * operator ; 
        select_ob_info *left ; 
        select_ob_info * right ; 
        float *float_val ; 
        unsigned char * blob ; 
        int * num_value ; 
        char * char_value ; 
        int acc_reg ; 
    }

    typedef struct table_range{
        int * range ; 
        int table_name ; 
        int col_name ; 
    }

    typedef struct table_and_col_hash_info{
        int * table_num ; 
        int * column_used[300] ; 
        int col_conter ; 
        table_range * range[300] ; 
        int range_counter ; 
    }

    typedef struct join_extra_info{
        table_and_col_hash_info * join_select_unique_table[300] ; 
        table_and_col_hash_info * join_select_hash_unique_table[300] ; 
        int * tables_occuring_number_of_times[300] ; 
        int join_table_counter  ; 
        int join_hash_counter ; 
    }

    typedef struct select_info{
        select_select_info *sel[300] ;
        int col_counter ;  
        bool select_agg ; 
        select_from_info *from[300] ; 
        int tables_counter ; 
        tables_list * tab ; 
        select_from_info *where ; 
        select_select_info *groupby[300] ;
        int groupby_counter ; 
        int * gb_select_unique[300] ; 
        int * hash[300] ; 
        int sel_uni_counter ; 
        select_from_info *having ; 
        select_ob_info *orderby[300] ; 
        int orderby_counter ; 
        join_extra_info * join[300] ; 
        int join_counter ; 

    }





    typedef struct sql_master {
        uint32_t root_page_num ; 
    }

    typedef struct col_det{
        char *name ; 
        int data_type ; 
    }

    typedef struct table  {
        char *name ; 
        int num_of_columns ; 
        col_det col[300] ; 
        int root_page_num ; 
    }

    tree * select ; 
    tree *from ; 

    int is_blob_literal(char *temp) {
        if ((temp[0] == 'X' || temp[0] == 'x') && temp[1] == '\'') {
            return 1;
        }
        return 0;
    }

    int  data_type_check( char *str) {
        char *endptr;

        strtol(str, &endptr, 10);
        if (*endptr == '\0' && endptr != str){
             return 0 ;
        }

        strtod(str, &endptr);
        if (*endptr == '\0' && endptr != str){
            return 1 ; 
        } 

        if (is_blob_literal(str)){
            return 2 ; 
        }
        return 3 ;
    }

    select_select_info *expre(select_select_info *ans ,    compiler *c , tree * temp){
        int i = 0 ; 
        while (i < temp->num){
            if (strcmp(temp->children[i]->comp , "+")== 0 || strcmp(temp->children[i]->comp, "-")== 0 || strcmp(temp->children[i]->comp, "*")== 0 || strcmp(temp->children[i]->comp, "/")== 0 || strcmp(temp->children[i]->comp , "=")== 0 || strcmp(temp->children[i]->comp, "!=")== 0 || strcmp(temp->children[i]->comp , ">")== 0 || strcmp(temp->children[i]->comp , ">=")== 0 || strcmp(temp->children[i]->comp , "<")== 0 || strcmp(temp->children[i]->comp, "<=")== 0 || strcmp(temp->children[i]->comp , "GROUP_CONCAT")== 0 || strcmp(temp->children[i]->comp , "MAX") == 0 || strcmp(temp->children[i]->comp , "MIN") == 0 || strcmp(temp->children[i]->comp , "COUNT") == 0 || strcmp(temp->children[i]->comp, "AVG") == 0 || strcmp(temp->children[i]->comp , "SUM") == 0){
                   ans->operator = temp->children[i]->comp;
                if (ans->left == NULL ){
                    ans->left = malloc(sizeof(select_select_info))
                    expre(ans->left, c, temp->children[i]);
                }
                else {
                    ans->right = malloc(sizeof(select_select_info))
                    expre(ans->right, c, temp->children[i]);
                }
            }
            else if (col_name_to_int_main(temp->children[i]->comp, c->select) != -1 ){
                if (ans->col_name != NULL ){
                     ans->extra_col = temp->children[i]->comp ;
                }
                else { 
                    ans->col_name = temp->children[i]->comp ;
                 }
             }
            else { 
                int check = data_type_check(temp->children[i]->comp);
                if (check == 0){
                    ans->num_value = atoi(temp->children[i]->comp);
                }
                else if (check == 1){
                    ans->float_val = (float)atof(temp->children[i]->comp);
                }
                else if (check == 2){
                    ans->blob = temp->children[i]->comp ;
                }
                else {
                    ans->char_value = temp->children[i]->comp ;
                }
            }
        i++;
        }
        return ans ; 
    }

    select_join_info *join_expre(select_join_info *ans ,    compiler *c , tree * temp){
        int i = 0 ; 
        while (i < temp->num){
            if (strcmp(temp->children[i]->comp , "+")== 0 || strcmp(temp->children[i]->comp, "-")== 0 || strcmp(temp->children[i]->comp, "*")== 0 || strcmp(temp->children[i]->comp, "/")== 0 || strcmp(temp->children[i]->comp , "=")== 0 || strcmp(temp->children[i]->comp, "!=")== 0 || strcmp(temp->children[i]->comp , ">")== 0 || strcmp(temp->children[i]->comp , ">=")== 0 || strcmp(temp->children[i]->comp , "<")== 0 || strcmp(temp->children[i]->comp, "<=")== 0 || strcmp(temp->children[i]->comp , "GROUP_CONCAT")== 0 || strcmp(temp->children[i]->comp , "MAX") == 0 || strcmp(temp->children[i]->comp , "MIN") == 0 || strcmp(temp->children[i]->comp , "COUNT") == 0 || strcmp(temp->children[i]->comp, "AVG") == 0 || strcmp(temp->children[i]->comp , "SUM") == 0){
                   ans->operator = temp->children[i]->comp;
                if (ans->left == NULL ){
                    ans->left = malloc(sizeof(select_join_info))
                    expre(ans->left, c, temp->children[i]);
                }
                else {
                    ans->right = malloc(sizeof(select_join_info))
                    expre(ans->right, c, temp->children[i]);
                }
            }
            else if (col_name_to_int_main(temp->children[i]->comp, c->select) != -1 ){
                if (ans->table_or_col_name != NULL ){
                     ans->extra_table_or_col_name = temp->children[i]->comp ;
                }
                else { 
                    ans->table_or_col_name = temp->children[i]->comp ;
                 }
             }
            else { 
                int check = data_type_check(temp->children[i]->comp);
                if (check == 0){
                    ans->num_value = atoi(temp->children[i]->comp);
                }
                else if (check == 1){
                    ans->float_val = (float)atof(temp->children[i]->comp);
                }
                else if (check == 2){
                    ans->blob = temp->children[i]->comp ;
                }
                else {
                    ans->char_value = temp->children[i]->comp ;
                }
            }
        i++;
        }
        return ans ; 
    }

    select_ob_info *expre_order_by(select_ob_info *ans ,    compiler *c , tree * temp){
        int i = 0 ; 
        while (i < temp->num){
            if (strcmp(temp->children[i]->comp , "+")== 0 || strcmp(temp->children[i]->comp, "-")== 0 || strcmp(temp->children[i]->comp, "*")== 0 || strcmp(temp->children[i]->comp, "/")== 0 || strcmp(temp->children[i]->comp , "=")== 0 || strcmp(temp->children[i]->comp, "!=")== 0 || strcmp(temp->children[i]->comp , ">")== 0 || strcmp(temp->children[i]->comp , ">=")== 0 || strcmp(temp->children[i]->comp , "<")== 0 || strcmp(temp->children[i]->comp, "<=")== 0 || strcmp(temp->children[i]->comp , "GROUP_CONCAT")== 0 || strcmp(temp->children[i]->comp , "MAX") == 0 || strcmp(temp->children[i]->comp , "MIN") == 0 || strcmp(temp->children[i]->comp , "COUNT") == 0 || strcmp(temp->children[i]->comp, "AVG") == 0 || strcmp(temp->children[i]->comp , "SUM") == 0){
                   ans->operator = temp->children[i]->comp;
                if (ans->left == NULL ){
                    ans->left = malloc(sizeof(select_ob_info))
                    expre_order_by(ans->left, c, temp->children[i]);
                }
                else {
                    ans->right = malloc(sizeof(select_ob_info))
                    expre_order_by(ans->right, c, temp->children[i]);
                }
            }
            else if (col_name_to_int_main(temp->children[i]->comp, c->select) != -1 ){
                if (ans->col_name != NULL ){
                     ans->extra_ob_name = temp->children[i]->comp ;
                }
                else { 
                    ans->ob_name = temp->children[i]->comp ;
                 }
             }
            else { 
                int check = data_type_check(temp->children[i]->comp);
                if (check == 0){
                    ans->num_value = atoi(temp->children[i]->comp);
                }
                else if (check == 1){
                    ans->float_val = (float)atof(temp->children[i]->comp);
                }
                else if (check == 2){
                    ans->blob = temp->children[i]->comp ;
                }
                else {
                    ans->char_value = temp->children[i]->comp ;
                }
            }
        i++;
        }
        return ans ; 
    }

    void from_parser_to_struct(compiler * c  , tree * select ){
        select_from_info **from  = c->select->from ; 
        int temp = 0   ; 
        while (temp < select->num && strcmp(select->children[temp]->comp , "FROM") != 0 ){
            temp++ ; 
        }
        select = select->children[temp] ; 
        int i = 0 ; 
        c->select->table_counter = 0 ; 
            while (  i < select->num && strcmp(select->children[i]->comp , "WHERE") != 0 ){
                if (strcmp(select->children[i]->comp , "JOIN") == 0 || strcmp(select->children[i]->comp , "INNER JOIN") == 0 || strcmp(select->children[i]->comp , "LEFT JOIN") == 0  || strcmp(select->children[i]->comp , "RIGHT JOIN") == 0  || strcmp(select->children[i]->comp , "CROSS JOIN") == 0  || strcmp(select->children[i]->comp , "FULL OUTER JOIN") == 0  ){
                    int num = c->select->tables_counter - 1  ; 
                    select_join_info * join = c->select->children[num]->join ; 
                    if (elect->children[i]->children[0] != NULL) { 
                        join->table_or_col_name = select->children[i]->children[0]->comp ; 
                        if (select->children[i]->children[0]->as != NULL ){
                            join->as = select->children[i]->children[0]->as ; 
                        }
                    }
                    if (select->children[i]->children[1] != NULL ){
                        join->left = malloc(sizeof(select_join_info)) ; 
                        join->left = join_expre( join->left  , c ,select->children[i]->children[1]  ) ; 
                    }
                }
                else { 
                    from[c->select->tables_counter] = malloc(sizeof(select_from_info)) ; 
                    from[c->select->tables_counter]->table_name = select->children[i]->comp ; 
                    if (select->as != NULL ){
                        from[c->select->tables_counter]->as = select->as ; 
                    }
                    c->select->tables_counter++ ; 
                }

                i++ ; 
            }
    }

    void select_parser_to_struct(  compiler *c , tree * select ){
        select_select_info *sel = c->select->sel ; 
        c->select.col_counter = 0 ; 
        int i = 0 ; 
        while ( i < select->num ){
            from_parser_to_struct(c , select ) ; 
            while(strcmp(select->children[i]->comp  , "FROM") != 0  ){
                if (1){
                    if (strcmp(select->children[i]->comp  , "*") == 0 ){
                        for (int k = 0 ; k < c->tables_counter ; k++ ){
                            table * temp = lookup_table(c->tl , c->select.from[k] ) ; 
                            for ( int j = 0 ; j < temp->num_of_columns ; j++ ){
                                sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                sel[c->select.col_counter ]->col_name = temp->col[j].name ; 
                                sel[c->select.col_counter ]->operator = NULL ; 
                                sel[c->select.col_counter ]->left = NULL ; 
                                sel[c->select.col_counter ]->right = NULL ; 
                                if (select->as != NULL ){
                                    sel[i]->as = select->as ; 
                                }
                                c->select.col_counter ++ ; 
                            }
                        }
                    }   
                    else  {
                        if (strcmp(select->children[i]->comp , "+")== 0 || strcmp(select->children[i]->comp, "-")== 0 || strcmp(select->children[i]->comp , "*")== 0  || strcmp(select->children[i]->comp , "/")== 0 || strcmp(select->children[i]->comp, "=")== 0 || strcmp(select->children[i]->comp , "!=")== 0  || strcmp(select->children[i]->comp, ">")== 0 ||  strcmp(select->children[i]->comp , ">=")== 0  || strcmp(select->children[i]->comp , "<")== 0 ||  strcmp(select->children[i]->comp , "<=")== 0 || strcmp(select->children[i]->comp , "GROUP_CONCAT")== 0 || strcmp(select->children[i]->comp, "MAX") == 0 || strcmp(select->children[i]->comp , "MIN") == 0 || strcmp(select->children[i]->comp, "COUNT") == 0 || strcmp(select->children[i]->comp, "AVG") == 0 || strcmp(select->children[i]->comp , "SUM") == 0){
                                sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                sel[c->select.col_counter ] = expre(sel[c->select.col_counter ] , c , select->children[i] )  ; 
                                if (select->as != NULL ){
                                    sel[i]->as = select->as ; 
                                }
                                c->select.col_counter ++ ; 
                        }
                        else { 
                            if(col_name_to_int_main(select->children[i]->comp, c->select) != -1){
                                sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                sel[c->select.col_counter ]->col_name = select->children[i]->comp ; 
                                sel[c->select.col_counter ]->operator = NULL ; 
                                sel[c->select.col_counter ]->left = NULL ; 
                                sel[c->select.col_counter ]->right = NULL ; 
                                if (select->as != NULL ){
                                    sel[i]->as = select->as ; 
                                }
                                c->select.col_counter ++ ;  
                            }
                            else { 
                                int check = data_type_check(select->children[i]->comp);
                                if (check == 0){
                                    sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                    sel[c->select.col_counter ]->col_name =  NULL ; 
                                    sel[c->select.col_counter ]->operator = NULL ; 
                                    sel[c->select.col_counter ]->left = NULL ; 
                                    sel[c->select.col_counter ]->right = NULL ; 
                                    sel[c->select.col_counter ]->num_value = atoi(select->children[i]->comp);
                                    c->select.col_counter ++ ; 
                                }
                                else if (check == 1){
                                    sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                    sel[nc->select.col_counter um]->col_name =  NULL ; 
                                    sel[c->select.col_counter ]->operator = NULL ; 
                                    sel[c->select.col_counter ]->left = NULL ; 
                                    sel[c->select.col_counter ]->right = NULL ; 
                                    sel[c->select.col_counter]->float_val = (float)atof(select->children[i]->comp);
                                    c->select.col_counter ++ ; 

                                }
                                else if (check == 2){
                                    sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                    sel[c->select.col_counter ]->col_name =  NULL ; 
                                    sel[c->select.col_counter ]->operator = NULL ; 
                                    sel[c->select.col_counter ]->left = NULL ; 
                                    sel[c->select.col_counter ]->right = NULL ; 
                                    sel[c->select.col_counter]->blob = select->children[i]->comp ;
                                    c->select.col_counter ++ ; 

                                }
                                else {
                                    sel[c->select.col_counter ] = malloc(sizeof(select_select_info)) ; 
                                    sel[c->select.col_counter ]->col_name =  NULL ; 
                                    sel[c->select.col_counter ]->operator = NULL ; 
                                    sel[c->select.col_counter ]->left = NULL ; 
                                    sel[c->select.col_counter ]->right = NULL ; 
                                    sel[c->select.col_counter ]->char_value = select->children[i]->comp ;
                                    c->select.col_counter++ ; 
                                }
                            }
                        }
                    }
                }
            }
            if (strcmp(select->children[i]->comp  , "WHERE") == 0  ){
                select_parser_to_struct(c , select->children[i] ) ; 
            }


        }
    }

    void where_parser_to_struct(compiler *c , tree * select ){
        int i = 0 ; 
        while (i < select->num ){
            if (i == 0 ){
                select_select_info *temp ; 
                c->select->where = expre(temp , c , select->children[i] ) ; 
            }
            else if(strcmp(select->children[i]->comp  , "ORDER BY") == 0 ){
                tree * orderby = c->select->children[i] ;  
                select_ob_info *ob =  c->select->orderby ; 
                int k = 0 ; 
                while (  k < orderby->num ){
                 if (c->select->groupby_counter > 0 ){
                    if (orderby->children[k]->operator != NULL ){ 
                            ob[c->select->orderby_counter] = malloc(sizeof(select_select_info));
                            ob[c->select->orderby_counter]->ob_name = orderby->children[k] ; 
                            ob[c->select->orderby_counter]->operator = NULL;
                            ob[c->select->orderby_counter]->left = NULL;
                            ob[c->select->orderby_counter]->right = NULL;
                            if (orderby->children[k]->as != NULL) {
                                ob[c->select->orderby_counter]->as = orderby->children[k]->as;
                            }
                            if ( orderby->direction[k] ){
                                 ob[c->select->orderby_counter]->dir = orderby->direction[k] ;
                            }
                            if (k+ 1 < orderby->num ){
                                if ( strcmp(orderby->children[k+1]->comp , "NULLS" ) == 0  ){
                                    select_ob_info *ob_child = orderby->children[k] ; 
                                    ob_child->extra_info_ob->nulls = malloc(sizeof(orderby->children[k+1]->comp)) ; 
                                    memcpy(ob_child->extra_info_ob->nulls , orderby->children[k+1]->comp , sizeof(orderby->children[k+1]->comp) ) ; 
                                    if (orderby->children[k+1]->num > 0 ){
                                        ob_child->extra_info_ob->order = malloc(sizeof(orderby->children[k+1]->children[0]->comp)) ; 
                                        memcpy(ob_child->extra_info_ob->order  , orderby->children[k+1]->children[0]->comp , sizeof(orderby->children[k+1]->children[0]->comp) ) ; 
                                    }
                                    else { 
                                        ob_child->extra_info_ob->order = "FIRST"; 
                                    }
                                    k++ ; 
                                    continue  ; 
                                }
                            }

                            c->select->orderby_counter++;
                    }
                    else { 
                        if (strcmp(groupby->children[k]->comp, "+") == 0 || strcmp(groupby->children[k]->comp, "-") == 0 ||strcmp(groupby->children[k]->comp, "*") == 0 || strcmp(groupby->children[k]->comp, "/") == 0 || strcmp(groupby->children[k]->comp, "=") == 0 || strcmp(groupby->children[k]->comp, "!=") == 0 || strcmp(groupby->children[k]->comp, ">") == 0 || strcmp(groupby->children[k]->comp, ">=") == 0 || strcmp(groupby->children[k]->comp, "<") == 0 || strcmp(groupby->children[k]->comp, "<=") == 0 || strcmp(groupby->children[k]->comp, "GROUP_CONCAT") == 0 || strcmp(groupby->children[k]->comp, "MAX") == 0 || strcmp(groupby->children[k]->comp, "MIN") == 0 || strcmp(groupby->children[k]->comp, "COUNT") == 0 || strcmp(groupby->children[k]->comp, "AVG") == 0 || strcmp(groupby->children[k]->comp, "SUM") == 0) {
                            ob[c->select->orderby_counter] = malloc(sizeof(select_select_info));
                            ob[c->select->orderby_counter] = expre_order_by(ob[c->select->orderby_counter] , c , orderby->children[k] );
                             ob[c->select->orderby_counter]->operator = NULL;
                            ob[c->select->orderby_counter]->left = NULL;
                            ob[c->select->orderby_counter]->right = NULL;
                            if (orderby->children[k]->as != NULL) {
                                ob[c->select->orderby_counter]->as = orderby->children[k]->as;
                            }
                            if ( orderby->direction[k] ){
                                 ob[c->select->orderby_counter]->dir = orderby->direction[k] ;
                            }
                            if (k+ 1 < orderby->num ){
                                if ( strcmp(orderby->children[k+1]->comp , "NULLS" ) == 0  ){
                                    select_ob_info *ob_child = orderby->children[k] ; 
                                    ob_child->extra_info_ob->nulls = malloc(sizeof(orderby->children[k+1]->comp)) ; 
                                    memcpy(ob_child->extra_info_ob->nulls , orderby->children[k+1]->comp , sizeof(orderby->children[k+1]->comp) ) ; 
                                    if (orderby->children[k+1]->num > 0 ){
                                        ob_child->extra_info_ob->order = malloc(sizeof(orderby->children[k+1]->children[0]->comp)) ; 
                                        memcpy(ob_child->extra_info_ob->order  , orderby->children[k+1]->children[0]->comp , sizeof(orderby->children[k+1]->children[0]->comp) ) ; 
                                    }
                                    else { 
                                        ob_child->extra_info_ob->order = "FIRST"; 
                                    }
                                    k++ ; 
                                    continue  ; 
                                }
                            }

                            c->select->orderby_counter++;
                        }
                    }
                 }
                 else { 
                    if (orderby->children[k]->operator != NULL ){ 
                            ob[c->select->orderby_counter] = malloc(sizeof(select_select_info));
                            ob[c->select->orderby_counter]->ob_name = orderby->children[k] ; 
                            ob[c->select->orderby_counter]->operator = NULL;
                            ob[c->select->orderby_counter]->left = NULL;
                            ob[c->select->orderby_counter]->right = NULL;
                            if (orderby->children[k]->as != NULL) {
                                ob[c->select->orderby_counter]->as = orderby->children[k]->as;
                            }
                            if ( orderby->direction[k] ){
                                 ob[c->select->orderby_counter]->dir = orderby->direction[k] ;
                            }
                            if (k+ 1 < orderby->num ){
                                if ( strcmp(orderby->children[k+1]->comp , "NULLS" ) == 0  ){
                                    select_ob_info *ob_child = orderby->children[k] ; 
                                    ob_child->extra_info_ob->nulls = malloc(sizeof(orderby->children[k+1]->comp)) ; 
                                    memcpy(ob_child->extra_info_ob->nulls , orderby->children[k+1]->comp , sizeof(orderby->children[k+1]->comp) ) ; 
                                    if (orderby->children[k+1]->num > 0 ){
                                        ob_child->extra_info_ob->order = malloc(sizeof(orderby->children[k+1]->children[0]->comp)) ; 
                                        memcpy(ob_child->extra_info_ob->order  , orderby->children[k+1]->children[0]->comp , sizeof(orderby->children[k+1]->children[0]->comp) ) ; 
                                    }
                                    else { 
                                        ob_child->extra_info_ob->order = "FIRST"; 
                                    }
                                    k++ ; 
                                    continue  ; 
                                }
                            }
                            c->select->orderby_counter++;
                    }
                 }
                 k++ ; 
                }
            }
            else if (strcmp(select->children[i]->comp  , "CASE") == 0 ){

            }
            else if (strcmp(select->children[i]->comp  , "GROUP BY") == 0 ){
                tree * groupby = c->select->children[i] ; 
                int k = 0 ; 
                select_select_info *gb =  c->select->groupby ; 
                    while (k < groupby->num && strcmp(groupby->children[k]->comp , "HAVING") != 0  ){
                    if (strcmp(groupby->children[k]->comp, "+") == 0 || strcmp(groupby->children[k]->comp, "-") == 0 ||strcmp(groupby->children[k]->comp, "*") == 0 || strcmp(groupby->children[k]->comp, "/") == 0 || strcmp(groupby->children[k]->comp, "=") == 0 || strcmp(groupby->children[k]->comp, "!=") == 0 || strcmp(groupby->children[k]->comp, ">") == 0 || strcmp(groupby->children[k]->comp, ">=") == 0 || strcmp(groupby->children[k]->comp, "<") == 0 || strcmp(groupby->children[k]->comp, "<=") == 0 || strcmp(groupby->children[k]->comp, "GROUP_CONCAT") == 0 || strcmp(groupby->children[k]->comp, "MAX") == 0 || strcmp(groupby->children[k]->comp, "MIN") == 0 || strcmp(groupby->children[k]->comp, "COUNT") == 0 || strcmp(groupby->children[k]->comp, "AVG") == 0 || strcmp(groupby->children[k]->comp, "SUM") == 0) {
                        gb[c->select->groupby_counter] = malloc(sizeof(select_select_info));
                        gb[c->select->groupby_counter] = expre(gb[c->select->groupby_counter] , c, groupby->children[k]);
                        if (select->as != NULL) {
                            gb[c->select->groupby_counter]->as = select->as;
                        }
                        c->select.groupby_counter++;
                    }
                    else {
                        if (col_name_to_int_main(groupby->children[k]->comp, c->select) != -1) {
                            gb[c->select->groupby_counter] = malloc(sizeof(select_select_info));
                            gb[c->select->groupby_counter]->col_name = groupby->children[k]->comp;
                            gb[c->select->groupby_counter]->operator = NULL;
                            gb[c->select->groupby_counter]->left = NULL;
                            gb[c->select->groupby_counter]->right = NULL;
                            if (select->as != NULL) {
                                gb[c->select->groupby_counter]->as = select->as;
                            }
                            c->select->groupby_counter++;
                        }
                        else {
                            int check = data_type_check(groupby->children[k]->comp);
                            if (check == 0) {
                                gb[c->select->groupby_counter]  = malloc(sizeof(select_select_info));
                                gb[c->select->groupby_counter]->col_name = NULL;
                                gb[c->select->groupby_counter]->operator = NULL;
                                gb[c->select->groupby_counter]->left = NULL;
                                gb[c->select->groupby_counter]->right = NULL;
                                gb[c->select->groupby_counter]->num_value = atoi(groupby->children[k]->comp);
                                c->select->groupby_counter++;
                            }
                            else if (check == 1) {
                                gb[c->select->groupby_counter]  = malloc(sizeof(select_select_info));
                                gb[c->select->groupby_counter]->col_name = NULL;
                                gb[c->select->groupby_counter]->operator = NULL;
                                gb[c->select->groupby_counter]->left = NULL;
                                gb[c->select->groupby_counter]->right = NULL;
                                gb[c->select->groupby_counter]->float_val = (float)atof(groupby->children[k]->comp);
                                c->select->groupby_counter++;
                            }
                            else if (check == 2) {
                                gb[c->select->groupby_counter]  = malloc(sizeof(select_select_info));
                                gb[c->select->groupby_counter]->col_name = NULL;
                                gb[c->select->groupby_counter]->operator = NULL;
                                gb[c->select->groupby_counter]->left = NULL;
                                gb[c->select->groupby_counter]->right = NULL;
                                gb[c->select->groupby_counter]->blob =  groupby->children[k]->comp;
                                c->select->groupby_counter++;
                            }
                            else {
                                gb[c->select->groupby_counter]  = malloc(sizeof(select_select_info));
                                gb[c->select->groupby_counter]->col_name = NULL;
                                gb[c->select->groupby_counter]->operator = NULL;
                                gb[c->select->groupby_counter]->left = NULL;
                                gb[c->select->groupby_counter]->right = NULL;
                                gb[c->select->groupby_counter]->char_value = groupby->children[k]->comp;
                                c->select->groupby_counter++;
                            }
                        }
                    }

                    k++;
                }
                else if (strcmp(groupby->children[k]->comp , "HAVING") == 0  ){
                    select_select_info *temp ; 
                    c->select->having = expre(temp , c , groupby->children[k] ) ; 
                }
            }
            else if (strcmp(select->children[i]->comp  , "LIMIT") == 0 || strcmp(select->children[i]->comp  , "OFFSET") == 0  ){

            }

        }
    }


    int col_name_to_int( char * column_name , table * t ){
        for (int i = 0 ; i < t->num_of_columns ; i++ ){
           if ( strcmp(t->col[i].name , column_name ) == 0 ){
              return i ; 
           }
        }
        return -1  ; 
    }

    int col_name_to_int_main( char * column_name , select_info *sf  ){
        int num = -1 ; 
        for ( int i = 0 ; i < sf->tab->num_of_tables ; i++ ){
            int number = col_name_to_int(column_name ,sf->from[i]->table_name ) ; 
            if ( number  != -1  ){
                if (num != -1 ){
                    return -1 ;  
                }
                else {
                    num = number ; 
                }
            }  
        }
        return num ; 
    }

    typedef struct tables_list{
        table * tables[300] ; 
        int num_of_tables ; 
    }


    int  table_num (tables_list * tab , char * table_name  ){
        for ( int i = 0 ; i < tab->num_of_tables ; i++ ){
           if ( strcmp( tab[i]->tables->name  , table_name  ) == 0 ) { 
                return i ; 
           }
        }
        return -1 ;  
    }

    char * table_name_from_num(tables_list * tab , int num ){
        return tab[i]->tables->name  ; 
    }

    table * lookup_table( tables_list * tab , char * table_name ){
        for ( int i = 0 ; i < tab->tables->num_of_tables ; i++ ){
           if ( strcmp( tab[i]->name  , table_name  ) == 0 ) { 
                return tab[i] ; 
           }
        }
        return NULL ; 
    }


    typedef struct orderby_key_cols{
        int  reg_num ; 
        int  direction ; 
        int  nullsfirst ; 
        int  nullslast  ; 
    }orderby_key_cols;


    unsigned char *  sorter_orderby_init(compiler * c  , int groupby ){
        orderby_key_cols * ob[300] ; 
        int ob_counter = 0 ; 
        int col = 0 ;
        for ( int i = 0 ; i < c->select->orderby_counter ; i++ ){
            if (c->select->orderby[i]->operator != NULL ){
                ob[i] = malloc(sizeof(orderby_key_cols));
                if (col == 0 ){
                    if (groupby == 0 ){
                        col = c->select->col_counter++ ; 
                    }
                    else { 
                        col = c->select->sel_uni_counter++ ; 
                    }

                }
                else if (col != 0 ) {
                    col = col + 1 ; 
                }

                ob[ob_counter]->reg_num = col ; 
                if (strcmp(c->select->orderby[i]->dir , "ASC") == 0 ){
                    ob[ob_counter]->direction = 0 ; 
                }
                else if (strcmp(c->select->orderby[i]->dir , "DSC") == 0 ) { 
                    ob[ob_counter]->direction = 1 ; 
                }
                if (c->select->orderby[i]->ex != NULL){
                    if (strcmp(c->select->orderby[i]->ex->order , "LAST") == 0 ){
                        ob[ob_counter]->nullslast = 1 ; 
                        ob[ob_counter]->nullsfirst = 0 ; 
                    }
                    else { 
                        ob[ob_counter]->nullsfirst = 1 ; 
                        ob[ob_counter]->nullslast = 0 ; 
                    }
                }
                else if (c->select->orderby[i]->ex == NULL ){
                        ob[ob_counter]->nullsfirst = 0 ; 
                        ob[ob_counter]->nullslast = 0 ; 
                }
                if (ob_counter < 300 ){
                    ob_counter++ ; 
                }

            }   
            else { 
                ob[i] = malloc(sizeof(orderby_key_cols));
                ob[ob_counter]->reg_num = col_name_to_int_main( c->select->orderby[i]->ob_name , c->select ) ; 
                if (col == 0 ){
                    if (groupby == 0 ){
                        col = c->select->col_counter++ ; 
                    }
                    else { 
                        col = c->select->sel_uni_counter++ ; 
                    }

                }
                else if (col != 0 ) {
                    col = col + 1 ; 
                }

                ob[ob_counter]->reg_num = col ; 
                if (strcmp(c->select->orderby[i]->dir , "ASC") == 0 ){
                    ob[ob_counter]->direction = 0 ; 
                }
                else if (strcmp(c->select->orderby[i]->dir , "DSC") == 0 ) { 
                    ob[ob_counter]->direction = 1 ; 
                }
                if (c->select->orderby[i]->ex != NULL){
                    if (strcmp(c->select->orderby[i]->ex->order , "LAST") == 0 ){
                        ob[ob_counter]->nullslast = 1 ; 
                        ob[ob_counter]->nullsfirst = 0 ; 
                    }
                    else { 
                        ob[ob_counter]->nullsfirst = 1 ; 
                        ob[ob_counter]->nullslast = 0 ; 
                    }
                }
                else if (c->select->orderby[i]->ex == NULL ){
                        ob[ob_counter]->nullsfirst = 0 ; 
                        ob[ob_counter]->nullslast = 0 ; 
                }
                if (ob_counter < 300 ){
                    ob_counter++ ; 
                }

            }
        }
        int struct_size = sizeof(orderby_key_cols);   
        int total_size = struct_size * ob_counter;
        unsigned char *buffer = malloc(total_size);
        int offset = 0;
        for (int i = 0; i < ob_counter ; i++) {
            memcpy(buffer + offset, ob[i], struct_size);
            offset += struct_size;
        }
        return buffer ; 
    }

    typedef extra_registers{
        int data_type ; 
        int lenght ; 
        union{
            long i ; 
            char *s ; 
            float r ; 
        }val ; 
    }

    void * just_for_extra_safety(compiler * c ){
        for ( int i = 0 ; i < 300 ; i++ ){
            c->join_select_hash_unique_table[i] = -1 ; 
        }
    }

    void * tables_and_their_hash_cursor_num(compiler * c  , select_select_info * from ){
        just_for_extra_safety(compiler * c ) ; 
        if (from->col_name ){
            int tab_num = table_num(c->tl ,table_thing(from->col) ) ; 
            int col_num = col_name_to_int(operand_thing(from->col) ,c->select ) ; 
            if ( c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->table_num != tab_num  ){
                c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->table_num = tab_num   ; 
                if ( c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->column_used[col_num] != col_num  ){
                    c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->column_used[col_num] = col_num  ;
                    c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->col_conter++ ; 
                }
                c->select->join[c->select->join_counter]->tables_occuring_number_of_times[tab_num]++ ; 
            }
        }
        else if (from->extra_col ){
            int tab_num = table_num(c->tl ,table_thing(from->extra_col)) ; 
            int col_num = col_name_to_int(operand_thing(from->extra_col) ,c->select ) ; 
            if ( c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num] != tab_num  ){
                c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num] = tab_num   ; 
                if ( c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->column_used[col_num] != col_num  ){
                     c->select->join[c->select->join_counter]->join_select_hash_unique_table[tab_num]->column_used[col_num] = col_num  ;
                }
                c->select->join[c->select->join_counter]->tables_occuring_number_of_times[tab_num]++ ; 
            }
        }
        else if(from->left != NULL ){
            tables_and_their_cursor_num( c , from->left ) ; 
        }
        else if(from->right != NULL ){
            tables_and_their_cursor_num( c , from->right ) ; 
        }
    }

    int tables_and_thier_cursor_num(compiler * c  , select_select_info *  from ){
        tables_and_their_hash_cursor_num(c , from ) ; 
        int counter = 0 ; 
        for (int i = 0 ; i < 300 ; i++ ){
            if (c->select->join[c->select->join_counter]->join_select_hash_unique_table[i] = i ){
                c->select->join[c->select->join_counter]->join_select_unique_table[counter]->table_num = i ; 
                for (int j = 0 ; j < c->select->join[c->select->join_counter]->join_select_hash_unique_table[i]->col_conter  ; j++ ){
                    c->select->join[c->select->join_counter]->join_select_unique_table[counter]->column_used[j] = c->select->join[c->select->join_counter]->join_select_unique_table[i]->column_used[j]
                }
                c->select->join[c->select->join_counter]->tables_occuring_number_of_times[counter] = c->select->join[c->select->join_counter]->tables_occuring_number_of_times[i] ; 
                counter++ ; 
            }
        }
        return counter ; 
    }

    int primary_key_offset(dab*db , int table_num ){
        int i = 0 ; 
        while(i< db->master->entries[table_num]->num_columns ){
            if ( db->master->entries[table_num]->columns[i]->is_primary_key == true ){
                return i ; 
            }
            i++ ; 
        }
        return -1 ; 
    }
    int string_length(char * table_name){
        if (table_name == NULL ){
            return 0 ; 
        }
        int i = 0 ; 
        while ( table_name[i] != '\0'){
            i++ ; 
        }
        return i ; 
    }

    char * blob_of_data(compiler * c , int pk_1 , int pk_2  , int row_num , char * table_name){
        char * blob  ; 
        int temp = 0 ; 
        int hash_bins_num = (db->master->entries[table_num(table_name)]->num_rows / 100 )* 30  ; 
        memcpy(blob + temp , hash_bins_num , sizeof(int))  ; 
        temp += sizeof(int) ; 
        memcpy(blob + temp , pk_1 , sizeof(int))  ; 
        temp += sizeof(int) ; 
        memcpy(blob + temp , pk_2 , sizeof(int))  ; 
        temp += sizeof(int) ; 
        if (row_num != -1 ){
            memcpy(blob + temp , row_num , sizeof(int))  ; 
            temp += sizeof(int) ; 
        }
        if (table_name != NULL ){
            int str_len = string_length(table_name) ; 
            memcpy(blob + temp , str_len , sizeof(int))  ; 
            temp += sizeof(int) ; 
            memcpy(blob+temp , table_name , str_len) ; 
        }
        return blob ; 
    }

    void join_equal_clause( compiler * c  , select_select_info * from  , select_from_info * from_org ){
        hash_bins_str * hash_bins ; 
        int first_blob  ; 
        int second_blob ; 
        int hash_cursor  ; 
        int pirmary_key_first_loc ; 
        int primary_key_second_loc ; 
        c->select->join[c->select->join_counter]->join_table_counter = tables_and_thier_cursor(c , from ) ; 
        for ( int i = 0 ; i < num  ; i++ ){
            emit(c , open_read_op , c->cursor_num + i , /*i(need to fix it bruh )*/ ,  -1 , -1 , NULL    ) ; 
        }

        int loop_addr_hb = c->count ; 
            int first  ; 
            int first_table = table_name_from_num( c->tl , from_org->table_name)  ;  
            for ( int m = 0 ; m < c->select->join[c->select->join_counter]->join_table_counter ; m++ ){
                if (c->select->join[c->select->join_counter]->join_select_unique_table[m] == first  ){
                    first = m ; 
                    break ; 
                }
            }
            pirmary_key_first_loc = c->register_counter++ ; 
            emit(c , column_op , first , primary_key_offset( db , first_table ) , pirmary_key_first_loc , NULL) ; 

            int second_table = table_name_from_num( c->tl , from_org->join->table_or_col_name) ; 
            int second ;
            for ( int m = 0 ; m < c->select->join[c->select->join_counter]->join_table_counter ; m++ ){
                if (c->select->join[c->select->join_counter]->join_select_unique_table[m] == first  ){
                    second = m ; 
                    break ; 
                }
            }
            primary_key_second_loc = c->register_counter++ ; 
            emit(c , column_op , second , primary_key_offset( db , first_table ) ,primary_key_second_loc , NULL) ; 

        if (from->left != NULL ){
            int left_reg =  join_func(c , from->left) ; 
             hash_cursor = c->select->join[c->select->join_counter]->join_hash_counter++ ; 
            first_blob = blob_of_data( pirmary_key_first_loc, primary_key_second_loc, -1 , NULL ) ;
            emit(c , push_to_hash , hash_cursor ,  left_reg , -1 , first_blob )  ; 
        }
        if (from->right != NULL ){
            int right_reg =  join_func(c , from->right) ; 
             hash_cursor = c->select->join[c->select->join_counter]->join_hash_counter++ ; 
            second_blob = blob_of_data( pirmary_key_first_loc, primary_key_second_loc, -1 , NULL ) ; 
            emit(c , push_to_hash , hash_cursor ,  right_reg , -1 , second_blob )  ; 
        }
        if (from->col_name != NULL ){
             hash_cursor = c->select->join[c->select->join_counter]->join_hash_counter++ ; 
            char * needed_table_char = table_thing(from->col_name) ; 
            table * needed_table = lookup_table(needed_table_char) ; 
            int needed_col = col_name_to_int( operand_thing(from->col_name , needed_table ) )  ; 
            emit(c , column_op , table_num(needed_table_char) , needed_col ,  c->register_counter , NULL ) ; 
            c->register_counter++ ; 
            first_blob = blob_of_data(pirmary_key_first_loc , primary_key_second_loc , needed_col , needed_table_char ) ; 
            emit(c , push_to_hash , hash_cursor ,  c->register_counter -1  , -1 , first_blob )  ; 
        }
        if (from->extra_col != NULL  ){
             hash_cursor = c->select->join[c->select->join_counter]->join_hash_counter++ ; 
            char * needed_table_char = table_thing(from->extra_col) ; 
            table * needed_table = lookup_table(needed_table_char) ; 
            int needed_col = col_name_to_int( operand_thing(from->extra_col , needed_table ) )  ; 
            emit(c , column_op , table_num(needed_table_char) , needed_col ,  c->register_counter , NULL ) ; 
            c->register_counter++ ; 
            second_blob = blob_of_data( pirmary_key_first_loc, primary_key_second_loc , needed_col , needed_table_char ) ; 
            emit(c , push_to_hash , hash_cursor ,  c->register_counter -1  , -1 , second_blob )  ; 
        }

        for ( int i = 0 ; i < num  ; i++ ){
            if (i == num -1 ){
                emit(c , next_cursor , c->cursor_num + i  , loop_addr_hb   , -1 , NULL ) ; 
            }
            else { 
                emit(c , next_cursor , c->cursor_num + i  , c->count + 1    , -1 , NULL ) ;     
            }
        }

        hash_cursor ;
        hash_cursor - 1 ; 


    }

    int check_the_stuff(compiler * c , select_from_info * from_org  , char * table ){
        if (strcmp(from_org , table ) == 0 ){
            return 1 ; 
        }
        if (strcmp(from_org->join->table_or_col_name , table ) == 0 ){
            return 1 ; 
        }
        return 0 ; 
    }

    void* get_the_max_and_min(compiler * c  ){
        c->select->join[c->select->join_counter]->join_select_unique_table ; 
        void * temp_max = "MAX" ; 
        void * temp_min = "MIN" ; 
        int init_register_counter = c->register_counter ; 
        for ( int i = 0 ; i < c->table_counter  ; i++ ){
            emit(c , open_read_op , c->cursor_num + i , /*i(need to fix it bruh )*/ ,  -1 , -1 , NULL    ) ; 
            emit(c , aggregate_init , c->register_counter++  , -1 , -1 , NULL  ) ; 
            emit(c , aggregate_init , c->register_counter++ , -1 , -1 , NULL  ) ;  
        }
        int end_init_register_counter = c->register_counter -1 ;
        int loop_addrese = c->count ; 
        for ( int i = 0 ; i < c->table_counter ; i++ ){
            if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->col_conter > 0 ){
                for ( int j  = 0 ; j < c->select->join[c->select->join_counter]->join_select_unique_table[i]->col_conter ; j++  ){
                    emit(c , column_op  , c->cursor_num + i , operand_thing(c->select->join[c->select->join_counter]->join_select_unique_table[i]->column_used[j] ) , c->register_counter++ , NULL ) ; 
                    emit(c , aggregate_step , init_register_counter +  2*i  , c->register_counter - 1 , -1 , temp_max  ) ; 
                    emit(c , aggregate_step , init_register_counter +  2*i + 1    , c->register_counter - 1 , -1 , temp_min  ) ; 
                }
            }
        }
        for ( int i = 0 ; i < c->table_counter  ; i++ ){
            if (i == c->table_counter -1 ){
                 emit(c , next_cursor , c->current_num + i  , loop_addrese , -1 , NULL  ) ; 
            }
            else { 
                 emit(c , next_cursor , c->current_num + i  , -1 , -1 , NULL  ) ; 
            }

        }

        for ( int i = 0 ; i < c->table_counter ; i++ ){
            if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->col_conter > 0 ){
                for ( int j  = 0 ; j < c->select->join[c->select->join_counter]->join_select_unique_table[i]->col_conter ; j++  ){
                    emit(c , aggregate_final , init_register_counter +  2*i  , -1  , MAX - 6 , temp_max ) ; 
                    emit(c , aggregate_final , init_register_counter +  2*i + 1  , -1  , MAX - 7 , temp_min ) ; 
                    int * ans[2] = malloc(2 * sizeof(int)); ; 
                    ans = {init_register_counter +  2*i , init_register_counter +  2*i + 1  } ; 
                    c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[c->select->join[c->select->join_counter]->join_select_unique_table[i]->range_counter++]->range = ans ; 
                    c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[c->select->join[c->select->join_counter]->join_select_unique_table[i]->range_counter++]->table_name = i ; 
                    c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[c->select->join[c->select->join_counter]->join_select_unique_table[i]->range_counter++]->col_name = j ; 
                }
            }
        }
    }

    int * take_care_of_expression(compiler * c , int target , select_select_info * from   , int reg_1 , int reg_2  , int * done  ){
        int *ans[2]  = malloc(2* sizeof(int)) ; 
        if (from->left != NULL ){
            ans = take_care_of_expression(c , target ,  from->left , reg_1  , reg_2 , done  ) ; 
            if (*done){
                return ans; 
            } 

        }
        if (from->right != NULL ){
            ans = take_care_of_expression(c , target ,  from->right , reg_1 , reg_2 , done  ) ; 
            if (*done){
                return ans; 
            } 
        }

        if (from->extra_col != NULL){
            int extra_num = table_num(table_thing(from->extra_col )) % c->select->join[c->select->join_counter]->join_table_counter ; 
            int extra_col_num = col_name_to_int(operand_thing(from->extra_col)) ; 
            if (1){
                if (extra_num  == target ){
                    if (extra_num > 1 ){
                        c->select->join[c->select->join_counter]->tables_occuring_number_of_times[extra_num]-- ; 
                    }
                    else { 
                            *done = 1; 
                            int * ans_for_func = malloc(2*sizeof(int)) ; 
                            ans_for_func[0] = reg_1 ; 
                            ans_for_func[1] = reg_2 ; 
                            return ans_for_func ; 
                    }
                }
                else { 
                    if (c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num] > 0 ){
                        c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num]--  ; 
                    }
                }
           }
            if (from->col_name != NULL ){
                int num =  table_num(table_thing(from->col_name )) % c->select->join[c->select->join_counter]->join_table_counter ; 
                int col_num = col_name_to_int(operand_thing(from->col_name)) ; 
                if (num == target){
                    if (num > 1 ){
                        c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num]--  ; 
                    }
                    else { 
                            *done = 1; 
                            int * ans_for_func = malloc(2*sizeof(int)) ; 
                            ans_for_func[0] = reg_1 ; 
                            ans_for_func[1] = reg_2 ; 
                            return ans_for_func ; 
                    }
                }
                else { 
                    if (c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num] > 0 ){
                        c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num]--  ; 
                    }
                }
                int operator =  -1  ; 

                if (from->operator != NULL ){
                    if (strcmp(from->operator , "+")== 0 ){
                        operator = add_op ; 
                    }
                    else if (strcmp(from->operator , "-")== 0 ){
                        operator = subs_op ; 
                    }
                    else if  (strcmp(from->operator , "*")== 0 ){
                        operator = mul_op ; 
                    }   
                    else if  (strcmp(from->operator , "/")== 0 ){
                        operator = divide_op ; 
                    }
                    else if (strcmp(from->operator , "IS NULL")== 0 ){
                        operator = is_null ; 
                    }
                    else if(strcmp(from->operator , "IS NOT NULL")== 0 ){
                        operator = is_not_null ; 
                    }
                    int extra_values = malloc(2* sizeof(int)) ; 
                    for (int i = 0 ; i < c->select->join[c->select->join_counter]->join_select_unique_table[i]->range_counter ; i++){
                        if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->table_name == extra_num ){
                            if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->col_name == extra_col_num ){
                                extra_values = c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->range ; 
                                break ; 
                            }
                            else {
                                continue ; 
                            }
                        }
                        else { 
                            continue ; 
                        }
                    }

                    int normal_val = malloc(2* sizeof(int)) ; 
                    for (int i = 0 ; i < c->select->join[c->select->join_counter]->join_select_unique_table[i]->range_counter ; i++){
                        if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->table_name == num ){
                            if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->col_name == col_num ){
                                normal_val = c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->range ; 
                                break ; 
                            }
                            else {
                                continue ; 
                            }
                        }
                        else { 
                            continue ; 
                        }
                    }
                    emit(c , operator  , extra_values[0]  ,  normal_val[0]  , reg_1 , NULL ) ;
                    emit(c , operator  , extra_values[1] ,  normal_val[1]  , reg_2 , NULL ) ;
                    int * ans_for_func = malloc(2*sizeof(int)) ; 
                    ans_for_func[0] = reg_1 ; 
                    ans_for_func[1] = reg_2 ; 
                    return ans_for_func ; 
                }
                else { 
                    // error ; 
                }
                
            }
            else { 
                // error  ; 
            }
        }


        if (from->col_name != NULL) {
            int num = table_num(table_thing(from->extra_col )) % c->select->join[c->select->join_counter]->join_table_counter ; 
            int col_num = col_name_to_int(operand_thing(from->col_name)) ; 
            if (1){
                if (num  == target ){
                    if (num > 1 ){
                        c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num]-- ; 
                    }
                    else { 
                            *done = 1; 
                            int * ans_for_func = malloc(2*sizeof(int)) ; 
                            ans_for_func[0] = reg_1 ; 
                            ans_for_func[1] = reg_2 ; 
                            return ans_for_func ; 
                    } 
                }
                else {
                    if (num > 0 ){
                        c->select->join[c->select->join_counter]->tables_occuring_number_of_times[num]-- ; 
                    }
                    else { 
                        //error ; 
                    }
                }
            }
            if (from->operator != NULL ){
                if (strcmp(from->operator , "+")== 0 ){
                    operator = add_op ; 
                }
                else if (strcmp(from->operator , "-")== 0 ){
                    operator = subs_op ; 
                }
                else if  (strcmp(from->operator , "*")== 0 ){
                    operator = mul_op ; 
                }   
                else if  (strcmp(from->operator , "/")== 0 ){
                    operator = divide_op ; 
                }
                else if (strcmp(from->operator , "IS NULL")== 0 ){
                    operator = is_null ; 
                }
                else if(strcmp(from->operator , "IS NOT NULL")== 0 ){
                    operator = is_not_null ; 
                }
                if (ans == NULL ){
                    if (from->num_value != NULL ){
                        emit(c , integer_op , *from->num_value , reg_1 , -1  , NULL  ) ;   
                        emit(c , integer_op , *from->num_value , reg_2 , -1  , NULL  ) ;   
                    }
                    else if (from->char_value != NULL ){
                        emit(c , string_op ,-1 , reg_1 , -1  , (void*)from->char_value   ) ;   
                        emit(c , string_op ,-1 , reg_2 , -1  , (void*)from->char_value   ) ; 
                    }
                    else if (from->float_val != NULL ){
                        emit(c , real_op , -1, reg_1 , -1  , (void*)from->float_val   ) ;   
                        emit(c , real_op , -1, reg_2 , -1  , (void*)from->float_val   ) ;   
                    }
                    else if (from->blob != NULL ){
                        emit(c , blob_op ,-1 , reg_1 , -1  , (void*)from->blob   ) ;   
                        emit(c , blob_op ,-1 , reg_2 , -1  , (void*)from->blob   ) ;   
                    }
                }
                int normal_val = malloc(2* sizeof(int)) ; 
                for (int i = 0 ; i < c->select->join[c->select->join_counter]->join_select_unique_table[i]->range_counter ; i++){
                    if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->table_name == num ){
                        if (c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->col_name == col_num ){
                            normal_val = c->select->join[c->select->join_counter]->join_select_unique_table[i]->range[i]->range ; 
                            break ; 
                        }
                        else {
                            continue ; 
                        }
                    }
                    else { 
                        continue ; 
                    }
                }               
                 if (ans != NULL ){
                    emit(c , operator  , ans[0] , normal_val[0] , reg_1 , NULL ) ;
                    emit(c , operator  , ans[1] , normal_val[1] , reg_2 , NULL ) ;
                }
                else { 
                    emit(c , operator  , reg_1 , normal_val[0] , reg_1 , NULL ) ;
                    emit(c , operator  , reg_2 , normal_val[1] , reg_2 , NULL ) ;
                }
                int * ans_for_func = malloc(2*sizeof(int)) ; 
                ans_for_func[0] = reg_1 ; 
                ans_for_func[1] = reg_2 ; 
                return ans_for_func ; 
            }
            else { 
                if (ans != NULL ){
                    return ans  ; 
                }
                else { 
                    // error ; 
                }
            }

        }

        else {
            if (ans != NULL ){
                if (from->num_value != NULL ){
                    emit(c , integer_op , *from->num_value , reg_1 , -1  , NULL  ) ;   
                    emit(c , integer_op , *from->num_value , reg_2 , -1  , NULL  ) ;   
                }
                else if (from->char_value != NULL ){
                    emit(c , string_op ,-1 , reg_1 , -1  , (void*)from->char_value   ) ;   
                    emit(c , string_op ,-1 , reg_2 , -1  , (void*)from->char_value   ) ; 
                }
                else if (from->float_val != NULL ){
                    emit(c , real_op , -1, reg_1 , -1  , (void*)from->float_val   ) ;   
                    emit(c , real_op , -1, reg_2 , -1  , (void*)from->float_val   ) ;   
                }
                else if (from->blob != NULL ){
                    emit(c , blob_op ,-1 , reg_1 , -1  , (void*)from->blob   ) ;   
                    emit(c , blob_op ,-1 , reg_2 , -1  , (void*)from->blob   ) ;   
                }

                emit(c , operator  , reg_1 , normal_val[0] , reg_1 , NULL ) ;
                emit(c , operator  , reg_2 , normal_val[1] , reg_2 , NULL ) ;
                int * ans_for_func = malloc(2*sizeof(int)) ; 
                ans_for_func[0] = reg_1 ; 
                ans_for_func[1] = reg_2 ; 
                return ans_for_func ; 
            }
            else {
                //error ; 
            }
        }
        int * ans_for_func = malloc(2*sizeof(int)) ; 
        ans_for_func[0] = reg_1 ; 
        ans_for_func[1] = reg_2 ; 
        return ans_for_func ; 
    }

    


    void* join_inequality_clause(compiler * c, select_select_info * from  , select_from_info * from_org  ){
        c->select->join[c->select->join_counter]->join_table_counter = tables_and_thier_cursor(c , from ) ; 
        for ( int i = 0 ; i < c->table_counter  ; i++ ){
            emit(c , open_read_op , c->cursor_num + i , /*i(need to fix it bruh )*/ ,  -1 , -1 , NULL    ) ; 
        }
        tables_and_thier_cursor_num(c , from) ; 
        get_the_max_and_min( c ) ; 
        int loop_addr_hb = c->count ; 
            int first  ; 
            int first_table = table_name_from_num( c->tl , from_org->table_name)  ;  
            for ( int m = 0 ; m < c->select->join[c->select->join_counter]->join_table_counter ; m++ ){
                if (c->select->join[c->select->join_counter]->join_select_unique_table[m] == first  ){
                    first = m ; 
                    break ; 
                }
            }
            pirmary_key_first_loc = c->register_counter++ ; 
            emit(c , column_op , first , primary_key_offset( db , first_table ) , pirmary_key_first_loc , NULL) ; 

            int second_table = table_name_from_num( c->tl , from_org->join->table_or_col_name) ; 
            int second ;
            for ( int m = 0 ; m < c->select->join[c->select->join_counter]->join_table_counter ; m++ ){
                if (c->select->join[c->select->join_counter]->join_select_unique_table[m] == first  ){
                    second = m ; 
                    break ; 
                }
            }
            primary_key_second_loc = c->register_counter++ ; 
            emit(c , column_op , second , primary_key_offset( db , first_table ) ,primary_key_second_loc , NULL) ; 
        if (from->col_name != NULL ){
            int check  = check_the_stuff(c , from_org , table_thing(from->col_name)  ) ; 
            if (check == 0 ){

            }
            else {
                if ()
            }

        }
        if (from->extra_col != NULL ){
            int check  = check_the_stuff(c , from_org , table_thing(from->extra_col)  ) ; 
            if (check == 0 ){
                
            }
            else {
                if ()
            }

        }
        if (from->left != NULL ){

        }
        if (from->right != NULL ){

        }

    }
    // see where you need ot begin is the equal things in the join can be done easily byt the hash but for the rest of them the thing like suppose jsut read this prompt if you want in the nothing ( bhuiit) claude i cant really explain it so yeah work from here 
    void compile_select (compiler *c ){
        emit(c , begin_op  , -1 , -1 , -1 , NULL ) ; 
        int cursor = c->cursor_num++ ; 
        int cursor_for_sort_orderby  = 0 ; 
        emit(c , open_read_op , cursor , sql_master->page_num ,  -1 , -1 , NULL    ) ; 
        emit(c , rewind_cursor , cursor , -1 , -1 , -1 , NULL  ) ; 
        int register_num = c->register_counter++ ; 
        c->register_start = register_num ; 
        if (c->select->groupby_counter == 0 ){
            if (c->select->orderby_counter > 0 ){
                 cursor_for_sort_orderby = c->sorter_cursor++ ; 
                emit(c , sorter_open , cursor_for_sort_orderby , c->select->orderby_counter , -1 , sorter_orderby_init(c , 0 )  ) ; 
            }
            int loop_addr = c->count ; 
            emit(c , eq_op , where_func(c ,c->select->where , false  ) , -1  , MAX , "BINARY" ) ; 
            for ( int i = 0 ; i < c->select->col_counter ; i++  ){
                int num = col_name_to_int_main( c->select->sel[i]->col_name , c->select   ) ; 
                    if (c->select->sel[i]->operator == NULL ){
                        register_num = c->register_counter++ ; 
                        if (1){
                            select_select_info *node = c->select->sel[i] ; 
                            if (node->col_name != NULL ){
                                if (num != -1 ){
                                 emit(c , column_op ,cursor , num , register_num  , NULL  ) ;  
                                }
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , register_num , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , register_num , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, register_num , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , register_num , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                    }
                    else { 
                        if ( num != -1 ){
                            c->register_counter = normal_func(c ,c->select->sel[i]  , false ) ; 
                            c->register_counter++ ; 
                        }
                    }
            }
            if (c->select->orderby_counter > 0 ){
                int extra_depletion_record = 0 ; 
                for ( int l = 0 ; l < c->select->orderby_counter ; l++  ){
                    if (c->select->orderby[l]->operator != NULL){
                        c->register_counter = orderby_func_main(c , c->select->orderby[l] , false ) ; 
                        c->register_counter++ ; 
                        extra_depletion_record++ ; 
                    }
                }
                emit(c , make_record , c->register_start , c->register_counter , c->register_counter + 1 , NULL ) ; 
                emit(c , sorter_insert , cursor_for_sort_orderby ,  c->register_counter + 1  , -1  , NULL) ; 
                c->register_counter = c->register_counter - extra_depletion_record ; 
            }
            if (c->select->orderby_counter == 0 ){
                if (c->select->select_agg != true ){
                emit(c , result_row ,c->register_start , c->register_start + c->register_counter , -1  , -1 , NULL ) ; 
                }
            }
            c->typ[loop_addr].p2 = c->count ; 
            emit(c , next_cursor , cursor , loop_addr   , -1 , NULL ) ;
            if (c->select->orderby_counter == 0 ){
                if (c->select->select_agg != true ){
                    for ( int z = 0 ; z < c->select->col_counter ;z++ ){
                        int extra_stupid = 0 ; 
                        if (c->select->sel[z]->acc_reg != -1 ){
                            emit(c ,aggregate_final , c->select->sel[z]->acc_reg , -1 , c->register_start + z , NULL  ) ;
                        }
                    }
                emit(c , result_row ,c->register_start , c->register_start + c->register_counter , -1  , -1 , NULL ) ; 
                }
            }
            if (c->select->orderby_counter > 0 ) {
                emit(c , orderby_sort , cursor_for_sort_orderby , -1 , -1 , NULL ) ; 
                int orderby_sort_addr = c->count ; 
                int start_orderby_reg_counter = c->register_counter++ ; 
                emit(c , sorter_data , cursor_for_sort_orderby , c->register_counter , -1 , NULL ) ; 
                emit(c , result_row , start_orderby_reg_counter , start_orderby_reg_counter+1 , -1  , -1 , NULL ) ; 
                emit(c , sorter_next , cursor_for_sort_orderby , orderby_sort_addr , -1 , NULL ) ; 
            }
            emit(c, close_cursor_op , cursor, -1, -1, -1, NULL);
            emit(c, halt, -1, -1, -1, -1, NULL);
        }
        // so where you need to start is na like make the new campare function for the orderby as per ythe nulls and the direction and stuff make it not that hard 

        else {
            int loop_addr = c->count ; 
            emit(c , eq_op , where_func(c ,c->select->where , false  ) , -1  , MAX , "BINARY" ) ; 
            int cursor_for_sort_orderby  = 0 ;
            int cursor_sort = c->sorter_cursor++ ; 
            emit(c , sorter_open , cursor_sort,c->select->groupby_counter , -1 , { col_name_to_int_main(c->select->groupby[sel_uni_counter]->col_name   , c->select)} ) ; 
            get_all_select_stuff(c) ; 
            get_all_hash_covered_gb(c) ; 
            if (c->select->orderby_counter > 0 ){
                cursor_for_sort_orderby = c->sorter_cursor++   ; 
                emit(c , sorter_open , cursor_for_sort_orderby , c->select->orderby_counter , -1 , sorter_orderby_init(c , 1 )  ) ; 
            }
            int loop_addr_gb = c->count ; 
            sort_groupby(c) ;  
            emit(c , next_cursor , cursor , loop_addr_gb   , -1 , NULL ) ; 
            emit(c , rewind_cursor , cursor , -1 , -1 , -1 , NULL  ) ; 
            emit(c , sorter_sort , cursor_sort, -1 , -1 , NULL ) ; 
            emit(c , sorter_data , cursor_sort , MAX -1   , -1 , NULL ) ; 
            emit(c , gb_sorter_data , MAX - 1  , c->select->sel_uni_counter , MAX - 1 , NULL    ) ; 
            int sorter_next_jump = c->count ; 
            emit(c , sorter_next , cursor_sort, -1 , -1 , NULL ) ; 
            emit(c , sorter_data ,cursor_sort , MAX - 2    , -1 , NULL ) ; 
            emit(c ,gb_sorter_data , MAX - 2    , c->select->sel_uni_counter , MAX - 2  , NULL    ) ; 
            emit(c , ne_op , MAX - 2   , -1 ,  MAX - 1  , NULL  ) ; 
                int addrwe = c->count ; 
                for ( int i = 0 ; i < c->select->groupby_counter ; i++  ){
                    int num = col_name_to_int_main( c->select->groupby[i]->col_name , c->select   ) ; 
                        if (c->select->groupby[i]->operator == NULL ){
                            register_num = c->register_counter++ ; 
                            if (1){
                                select_select_info *node = c->select->groupby[i] ; 
                                if (node->col_name != NULL ){
                                    if (num != -1 ){
                                    emit(c , gb_specific_column_op , MAX - 1 , num , register_num  , NULL  ) ;  
                                    }
                                }
                                else {
                                    if (node->num_value != NULL ){
                                        emit(c , integer_op , *node->num_value , register_num , -1  , NULL  ) ;   
                                    }
                                    else if (node->char_value != NULL ){
                                        emit(c , string_op ,-1 , register_num , -1  , (void*)node->char_value   ) ;   
                                    }
                                    else if (node->float_val != NULL ){
                                        emit(c , real_op , -1, register_num , -1  , (void*)node->float_val   ) ;   
                                    }
                                    else if (node->blob != NULL ){
                                        emit(c , blob_op ,-1 , register_num , -1  , (void*)node->blob   ) ;   
                                    }
                                }
                            }
                        }
                        else { 
                            if ( num != -1 ){
                                 c->register_counter =  groupby_func(c ,c->select->groupby[i]  , true ) ; 
                                 c->register_counter+ ; 
                            }
                        }
                }
                emit(c , copy_op , MAX - 2  , MAX -1  , -1 , NULL ) ; 
                int having   = 0 ; 
                if ( c->select->having != NULL  ){
                    if (c->select->having->acc_reg != -1 ){
                        having = where_func(c ,c->select->having  , true   ) ; 
                    }
                    else { 
                        having = where_func(c ,c->select->having  , false    ) ;    
                    }

                }
                emit(c , eq_op , having , -1 ,  MAX , NULL  ) ; 
                int gb_hav = c->count ; 
                if (c->select->orderby_counter > 0 ){
                    int extra_depletion_record = 0 ; 
                    for ( int l = 0 ; l < c->select->orderby_counter ; l++  ){
                        if (c->select->orderby[l]->operator != NULL){
                            c->register_counter = orderby_func_main(c , c->select->orderby[l] , false ) ; 
                            c->register_counter++ ; 
                            extra_depletion_record++ ; 
                        }
                    }
                    emit(c , make_record , c->register_start , c->register_counter , c->register_counter + 1 , NULL ) ; 
                    emit(c , sorter_insert , cursor_for_sort_orderby ,  c->register_counter + 1  , -1  , NULL) ; 
                    c->register_counter = c->register_counter - extra_depletion_record ; 
                }
                emit(c , result_row ,c->register_start , c->register_start + c->register_counter , -1  , -1 , NULL ) ; 
                c->typ[gb_hav].p2 = c->count ; 
                emit(c ,goto_op , -1 ,  sorter_next_jump  , -1 , NULL ) ; 
                c->typ[addrwe].p2 = c->count ; 
                for ( int i = 0 ; i < c->select->groupby_counter ; i++  ){
                    int num = col_name_to_int_main( c->select->groupby[i]->col_name , c->select   ) ; 
                        if (c->select->groupby[i]->operator != NULL ){
                            if ( num != -1 ){
                                 int not_needed = groupby_func(c ,c->select->groupby[i]  , false ) ; 
                            }
                        }

                }
                emit(c , copy_op , MAX - 2  , MAX -1  , -1 , NULL ) ; 
                emit(c ,goto_op , -1 ,  sorter_next_jump  , -1 , NULL ) ; 

            c->typ[sorter_next_jump + 1].p2 = c->count ; 
            for ( int i = 0 ; i < c->select->groupby_counter ; i++  ){
                int num = col_name_to_int_main( c->select->groupby[i]->col_name , c->select   ) ; 
                    if (c->select->groupby[i]->operator == NULL ){
                        register_num = c->register_counter++ ; 
                        if (1){
                            select_select_info *node = c->select->groupby[i] ; 
                            if (node->col_name != NULL ){
                                if (num != -1 ){
                                emit(c , gb_specific_column_op , MAX - 1 , num , register_num  , NULL  ) ;  
                                }
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , register_num , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , register_num , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, register_num , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , register_num , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                    }
                    else { 
                        if ( num != -1 ){
                                c->register_counter =  groupby_func(c ,c->select->groupby[i]  , true ) ; 
                                c->register_counter++ ; 
                        }
                    }
            }
            emit(c , eq_op , having , -1 ,  MAX , NULL  ) ; 
            int gb_hav_fin = c->count ; 
            if (c->select->orderby_counter > 0 ){
                int extra_depletion_record = 0 ; 
                for ( int l = 0 ; l < c->select->orderby_counter ; l++  ){
                    if (c->select->orderby[l]->operator != NULL){
                        c->register_counter = orderby_func_main(c , c->select->orderby[l] , false ) ; 
                        c->register_counter++ ; 
                        extra_depletion_record++ ; 
                    }
                }
                emit(c , make_record , c->register_start , c->register_counter , c->register_counter + 1 , NULL ) ; 
                 emit(c , sorter_insert , cursor_for_sort_orderby ,  c->register_counter + 1  , -1  , NULL) ; 
                c->register_counter = c->register_counter - extra_depletion_record ; 
            }
            if (c->select->orderby_counter > 0 ) {
                emit(c , orderby_sort , cursor_for_sort_orderby , -1 , -1 , NULL ) ; 
            }
            if (c->select->orderby_counter == 0 ){
                emit(c , result_row ,c->register_start , c->register_start + c->register_counter , -1  , -1 , NULL ) ; 
            }
            c->typ[gb_hav_fin].p2 = c->count ; 
            if (c->select->orderby_counter > 0 ) {
                emit(c , orderby_sort , cursor_for_sort_orderby , -1 , -1 , NULL ) ; 
                int orderby_sort_addr = c->count ; 
                int start_orderby_reg_counter = c->register_counter++ ; 
                emit(c , sorter_data , cursor_for_sort_orderby , c->register_counter , -1 , NULL ) ; 
                emit(c , result_row , start_orderby_reg_counter , start_orderby_reg_counter+1 , -1  , -1 , NULL ) ; 
                emit(c , sorter_next , cursor_for_sort_orderby , orderby_sort_addr , -1 , NULL ) ; 
            }
            emit(c, close_cursor_op , cursor, -1, -1, -1, NULL) ;
            emit(c, halt, -1, -1, -1, -1, NULL) ;
         }
        
    }

    // one more boring stuff simply see liek here in the where na we have to check wheter the thing we have is 0 or 1 true or false if it is false you need to do the next command execute so for that one once you do the next command so now the thing is na we have the eq_op bytecode for the thing which like checks if the thing is  true or flase  then it like jumps to the next part the issue we dont know where the next_op thing will come in the execution so we simply put it as -1 and then we just updat ething thing when we find it simple as that 
    // okay one of the most insane boring thing which happens here is see man like the loop occurs in the bytecodes itself so when we like put the register_counter like see we did the thing and as soo nas we hit the next_op it calls the bytecoders which we passed on earleir the earleir one okay only that gets called we are not calling anything in the compile_seelct getting ti it is complelty different thing got it 

    int groupby_func(compiler * c , select_select_info * temp  , bool final ){
        int addk = c->register_counter  ; 
        int ans = group_by_func(c ,temp , final ) ; 
        c->register_counter = addk  ; 
        return ans ; 
    }

    void get_all_select_stuff(compiler * c ){
        int i = 0 ; 
        while ( i < c->select->col_counter ){
             int num = col_name_to_int_main( c->select->sel[i]->col_name , c->select  )  ; 
            if (num != -1 ){
                if (c->select->sel[i]->operator ==  NULL ){
                    if (c->select->hash[num] != num ){
                        c->select->hash[num] = num  ; 
                        c->select->gb_select_unique[c->select->sel_uni_counter++] = num ; 
                    }
                }
                else {
                    select_select_info *temp = c->select->sel[i] ; 
                    if (temp->operator != NULL ){
                        get_the_data_tree(temp , c->select) ; 
                    }
                }
            }
            i++ ;
        }
        return  ; 
    }

    void get_all_hash_covered_gb(compiler * c ){
        int i = 0 ; 
        int num ; 
        while ( i < c->select->groupby_counter){
            num = col_name_to_int_main( c->select->sel[i]->col_name  , c->select  )   ; 
            if (num != -1 ){
                if (c->select->groupby[i]->operator == NULL ){
                    if ( c->select->hash[num] != num  ){
                        c->select->hash[num] = num  ; 
                    }
                }
            }
            i++ ; 
        }
        return ; 
    }

    void get_the_tree_hash(select_select_info * temp , select_info *  sf ){
        int num = col_name_to_int_main( temp->col_name , sf  )  ; 
        int extra_num ; 
        if (num != -1 ){
            if (temp->left == NULL && temp->right == NULL  ){
                 extra_num = col_name_to_int_main( temp->extra_col , sf  )   ; 
                if ( sf->hash[num] != num ){
                    sf->hash[num] = num ; 
                }
                if (extra_num != -1 ){
                    if ( sf->hash[extra_num] != extra_num ){
                        sf->hash[extra_num] = extra_num  ; 
                    }
                 }
                return  ; 
            }

            else { 
                if ( sf->hash[num] != num ){
                    sf->hash[num] = num; 
                }
            }
        }

        if (temp->left != NULL ){
            get_the_tree_hash(temp->left , sf) ; 
        }
        if (temp->right != NULL ){
            get_the_tree_hash(temp->right , sf) ; 
        }

        return ; 
    }

    void get_the_data_tree(select_select_info * temp , select_info *  sf ){
        int num = col_name_to_int_main( temp->col_name , sf  )  ; 
        int extra_num ; 
        if (num != -1 ){
            if (temp->left == NULL && temp->right == NULL  ){
                 extra_num = col_name_to_int_main( temp->extra_col , sf  )   ; 
                if ( sf->hash[num] != num ){
                    sf->hash[num] = num  ; 
                    sf->gb_select_unique[sf->sel_uni_counter++ ] = num ; 
                }
                if (extra_num != -1 ){
                if ( sf->hash[extra_num] != extra_num ){
                    sf->hash[extra_num] = extra_num  ; 
                    sf->gb_select_unique[sf->sel_uni_counter++ ] = extra_num ; 
                }
             }
                return  ; 
            }

            else { 
                if ( sf->hash[num] != num ){
                    sf->hash[num] = num  ; 
                    sf->gb_select_unique[sf->sel_uni_counter++ ] = num ; 
                }
            }
        }

        if (temp->left != NULL ){
            get_the_data_tree(temp->left , sf) ; 
        }
         if (temp->right != NULL ){
            get_the_data_tree(temp->right , sf) ; 
        }

        return ; 
    }

    void sort_groupby(compiler * c  ){
        int i = 0 ; 
        int start = c->register_start + c->register_counter ; 
        int cur = start ; 
        int norm_cursor = c->cursor_num++  ; 
        while ( i < c->select->sel_uni_counter){
            emit(c , column_op ,norm_cursor , c->select->gb_select_unique[i] , cur  , NULL  ) ;    
            i++ ; 
            cur++ ; 
        }
        i = 0 ; 
        if (c->select->orderby_counter > 0 ){
            while ( i < c->select->orderby_counter){
                int num = col_name_to_int_main( c->select->orderby[i]->ob_name , c->select   )   ; 
                if (num != -1   ){
                    if (c->select->orderby[i]->operator != NULL ){
                        int temp = c->register_counter ; 
                        c->register_counter = cur ; 
                        c->register_counter = normal_func(c ,c->select->orderby[i]  , false  ) ; 
                        c->register_counter++ ; 
                        c->register_counter = temp ; 
                        cur++ ; 
                    }
                }
                else { 
                    break ; 
                }
                i++ ; 
            }
        }
        i = 0 ; 
        while ( i < c->select->groupby_counter){
            int num = col_name_to_int_main( c->select->groupby[i]->col_name , c->select   )   ; 
            if (num != -1   ){
                if (c->select->groupby[i]->operator != NULL ){
                    int temp = c->register_counter ; 
                    c->register_counter = cur ; 
                    c->register_counter = normal_func(c ,c->select->sel[i]  , false  ) ; 
                    c->register_counter++ ; 
                    c->register_counter = temp ; 
                }
                else { 
                    emit(c , column_op ,norm_cursor , num , cur  , NULL  ) ;  
                }
            }
            else { 
                break ; 
            }
            i++ ; 
            cur++ ; 
        }
        emit(c , make_record , start , cur , start , NULL ) ; 
        emit(c , sorter_insert , cursor_sort, start , -1  , NULL) ; 
    }

    // bug alert the aggregate init and the aggregate final pretty much break up everything in the thing itds just supported ffor the main stuff but for the function where the aggregate is pretty much used it breaks out there so yeah need to do it 
    void aggregate_select(compiler *c , select_select_info * node  ){
        void * operation = node->operator ; 
        int reg ; 
        if (node->col_name != NULL) {             
            int num = col_name_to_int_main(node->col_name, c->select);
            reg = c->register_counter++;
            emit(c, column_op, cursor, num, reg, NULL);
        }
        else if (node->left != NULL) {      
            c->register_counter = normal_func(c ,c->select->sel[i]  , false ) ; 
            c->register_counter++ ;        
           reg = func(c, node->left, cursor);
        }
        else {                         
            reg = -1;
        }
        emit(c , aggregate_step , node->acc_reg , reg , NULL , operation) ; 
    }
    int where_func(compiler *c , select_select_info * node , bool req ){
        int first_reg = c->register_counter ; 
        int ans = func(c , node , req ) ; 
        emit(c , integer_op , 0 , MAX , -1 , NULL ) ; 
        c->register_counter = first_reg ; 
        return ans ;  
    }

    int normal_func(compiler *c , select_select_info * node , bool req  ){
        int first_reg = c->register_counter ; 
        int ans = func(c , node , req ) ; 
        c->register_counter = first_reg ; 
        return ans ;  
    }

    int orderby_func_main(compiler *c , select_ob_info * node ){
        int first_reg = c->register_counter ; 
        int ans = orderby_func(c , node) ; 
        c->register_counter = first_reg ; 
        return ans ;  
    }

     // bug alert the func needs a new attribute known as final check it out i think its quite broken so yeah 
    int  func(compiler *c , select_select_info * node , bool final  ){
        int reg  = c->register_counter   ; 
        int operator ; 
        if (node->operator != NULL  ) {
            if (strcmp(node->operator , "+")== 0 ){
                operator = add_op ; 
            }
            else if (strcmp(node->operator , "-")== 0 ){
                operator = subs_op ; 
            }
            else if  (strcmp(node->operator , "*")== 0 ){
                operator = mul_op ; 
            }   
            else if  (strcmp(node->operator , "/")== 0 ){
                operator = divide_op ; 
            }
            else if  (strcmp(node->operator , "=")== 0 ){
                operator = eq_select_op ; 
            }
            else if  (strcmp(node->operator , "!=")== 0 ){
                 operator = ne_select_op ; 
            }
            else if (strcmp(node->operator , ">")== 0 ){
                 operator = gt_select_op ; 
            }
            else if  (strcmp(node->operator , ">=")== 0 ){
                    operator = ge_select_op ; 
            }
            else if  (strcmp(node->operator , "<")== 0 ){
                 operator = lt_select_op ; 
            }
            else if (strcmp(node->operator , "<=")== 0 ){
                 operator = le_select_op ; 
            }
            else if (strcmp(node->operator , "AND")== 0 ){
                 operator = and_op ; 
            }
            else if (strcmp(node->operator , "OR")== 0 ){
                 operator = or_op ; 
            }
            else if (strcmp(node->operator , "IS NULL")== 0 ){
                 operator = is_null ; 
            }
            else if(strcmp(node->operator , "IS NOT NULL")== 0 ){
                 operator = is_not_null ; 
            }
            else if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                c->select->select_agg = true ; 
                if (node->acc_reg == -1 ){
                    node->acc_reg = c->register_counter++   ; 
                    emit(c ,aggregate_init ,node->acc_reg , -1 , -1 , NULL  ) ; 
                }
                aggregate_select(c , node ) ; 
                if (final == true ){
                    emit(c ,aggregate_final ,node->acc_reg , -1 , node->acc_reg  , NULL  ) ; 
                    emit(c ,aggregate_reset , node->acc_reg  , -1 , -1 , NULL ) ; 
                }
            }
            else { 
                return reg  ; 
            }
            int num = col_name_to_int_main( node->col_name , c->select   ) ; 
            int cursor = c->cursor_num ; 


            if (node->right == NULL && node->left == NULL  ){
                if (operator != is_null  && operator != is_not_null ){
                    int reg_left = c->register_counter++ ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_left , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_left , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }

                    int reg_right =  c->register_counter++ ;    
                    if (1){
                        if (node->extra_col != NULL ){
                            int extra_num = col_name_to_int_main( node->extra_col , c->select   ) ; 
                            emit(c , column_op ,cursor , extra_num , reg_right  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_left ,reg_right , reg , -1 , NULL ) ;    
            }
            else {
                int reg_temp = c->register_counter++  ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }               
            }


            else if (node->right != NULL && node->left == NULL ) {
            if (operator != is_null  && operator != is_not_null ){
                int reg_right = func(c , node->right ,final  ) ; 
                int reg_left =  c->register_counter++ ;  
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_left , NULL  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_left , N-1ULL  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){p
                                    emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left, reg_right , reg , -1 , NULL ) ;   
            }
            else  {
                    int reg_temp = func(c , node->right ,final ) ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_temp  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }    

            }

            else if(node->left != NULL && node->right == NULL ){
            if (operator != is_null  && operator != is_not_null ){
                int reg_left = func(c , node->left,final  ) ; 
                int reg_right =  c->register_counter++ ;  
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , column_op ,cursor , num , reg_right  , NULL  ) ;  
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right  , reg , -1 , NULL ) ;  
            }
            else  {
                    int reg_temp = func(c , node->left ,final ) ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_temp  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }
           }
            else { 
                int reg_right = func(c , node->right ,final  ) ; 
                int reg_left = func(c , node->left,final  ) ; 
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right , reg , -1 , NULL ) ;  
            }
        }

        return reg ; 
    }


    int  group_by_func(compiler *c , select_select_info * node  , bool final ){
        int reg = c->register_counter    ; 
        int operator ; 
        if (node->operator != NULL  ) {
            if (strcmp(node->operator , "+")== 0 ){
                operator = add_op ; 
            }
            else if (strcmp(node->operator , "-")== 0 ){
                operator = subs_op ; 
            }
            else if  (strcmp(node->operator , "*")== 0 ){
                operator = mul_op ; 
            }   
            else if  (strcmp(node->operator , "/")== 0 ){
                operator = divide_op ; 
            }
            else if  (strcmp(node->operator , "=")== 0 ){
                operator = eq_select_op ; 
            }
            else if  (strcmp(node->operator , "!=")== 0 ){
                 operator = ne_select_op ; 
            }
            else if (strcmp(node->operator , ">")== 0 ){
                 operator = gt_select_op ; 
            }
            else if  (strcmp(node->operator , ">=")== 0 ){
                    operator = ge_select_op ; 
            }
            else if  (strcmp(node->operator , "<")== 0 ){
                 operator = lt_select_op ; 
            }
            else if (strcmp(node->operator , "<=")== 0 ){
                 operator = le_select_op ; 
            }
            else if (strcmp(node->operator , "AND")== 0 ){
                 operator = and_op ; 
            }
            else if (strcmp(node->operator , "OR")== 0 ){
                 operator = or_op ; 
            }
            else if (strcmp(node->operator , "IS NULL")== 0 ){
                 operator = is_null ; 
            }
            else if(strcmp(node->operator , "IS NOT NULL")== 0 ){
                 operator = is_not_null ; 
            }
            else if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                if (node->acc_reg == -1 ){
                    node->acc_reg = c->register_counter++   ; 
                    emit(c ,aggregate_init ,node->acc_reg , -1 , -1 , NULL  ) ; 
                }
                aggregate_select(c , node ) ; 
                if (final == true ){
                    emit(c ,aggregate_final ,node->acc_reg , -1 , node->acc_reg  , NULL  ) ; 
                    emit(c ,aggregate_reset , node->acc_reg  , -1 , -1 , NULL ) ; 
                }
            }
            else { 
                return reg  ; 
            }
            int num = col_name_to_int_main( node->col_name , c->select   ) ; 
            int cursor = c->cursor_num ; 


            if (node->right == NULL && node->left == NULL  ){
                if (operator != is_null  && operator != is_not_null ){
                    int reg_left = c->register_counter++ ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , gb_specific_column_op , MAX - 1 , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_left , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_left , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }

                    int reg_right =  c->register_counter++ ;    
                    if (1){
                        if (node->extra_col != NULL ){
                            int extra_num = col_name_to_int_main( node->extra_col , c->select   ) ; 
                            emit(c , gb_specific_column_op , MAX - 1 , extra_num , reg_right  , NULL  ) ;   
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_left ,reg_right , reg , -1 , NULL ) ;    
                }
                else {
                    int reg_temp = c->register_counter++  ; 
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , gb_specific_column_op , MAX - 1 , num , reg_temp  , NULL  ) ; 
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                        reg = c->register_counter++  ; 
                        emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

                }               
            }


            else if (node->right != NULL && node->left == NULL ) {
                if (operator != is_null  && operator != is_not_null ){
                    int reg_right = group_by_func(c , node->right  , final ) ; 
                    int reg_left =  c->register_counter++ ;  
                            if (1){
                                if (node->col_name != NULL ){
                                    emit(c , gb_specific_column_op , MAX - 1 , num , reg_left  , NULL  ) ; 
                                }
                                else {
                                    if (node->num_value != NULL ){
                                        emit(c , integer_op , *node->num_value , reg_left , -1  , NULL  ) ;   
                                    }
                                    else if (node->char_value != NULL ){
                                        emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                                    }
                                    else if (node->float_val != NULL ){
                                        emit(c , real_op , -1, reg_left , -1  , (void*)node->float_val   ) ;   
                                    }
                                    else if (node->blob != NULL ){
                                        emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                                    }
                                }
                            }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_left, reg_right , reg , -1 , NULL ) ;   
                }
                else  {
                        int reg_temp =  group_by_func(c , node->right  , final ) ; 
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , gb_specific_column_op , MAX - 1 , num , reg_temp  , NULL  ) ; 
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                        reg = c->register_counter++  ; 
                        emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

                }    

            }

        else if(node->left != NULL && node->right == NULL ){
            if (operator != is_null  && operator != is_not_null ){
                int reg_left =  group_by_func(c , node->left  , final ) ; 
                int reg_right =  c->register_counter++ ;  
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , gb_specific_column_op , MAX - 1 , num , reg_right  , NULL  ) ; 
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right  , reg , -1 , NULL ) ;  
            }
            else  {
                    int reg_temp =  group_by_func(c , node->left  , final ) ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , gb_specific_column_op , MAX - 1 , num , reg_temp  , NULL  ) ; 
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }
        }
        else { 
            int reg_right =  group_by_func(c , node->right  , final ) ; 
            int reg_left =  group_by_func(c , node->left  , final ) ; 
            reg = c->register_counter++  ; 
            emit(c , operator ,reg_left , reg_right , reg , -1 , NULL ) ;  
        }
        }
        return reg ; 
    }

    int  orderby_func(compiler *c , select_ob_info * node , bool final  ){
        int reg  = c->register_counter   ; 
        int operator ; 
        if (node->operator != NULL  ) {
            if (strcmp(node->operator , "+")== 0 ){
                operator = add_op ; 
            }
            else if (strcmp(node->operator , "-")== 0 ){
                operator = subs_op ; 
            }
            else if  (strcmp(node->operator , "*")== 0 ){
                operator = mul_op ; 
            }   
            else if  (strcmp(node->operator , "/")== 0 ){
                operator = divide_op ; 
            }
            else if  (strcmp(node->operator , "=")== 0 ){
                operator = eq_select_op ; 
            }
            else if  (strcmp(node->operator , "!=")== 0 ){
                 operator = ne_select_op ; 
            }
            else if (strcmp(node->operator , ">")== 0 ){
                 operator = gt_select_op ; 
            }
            else if  (strcmp(node->operator , ">=")== 0 ){
                    operator = ge_select_op ; 
            }
            else if  (strcmp(node->operator , "<")== 0 ){
                 operator = lt_select_op ; 
            }
            else if (strcmp(node->operator , "<=")== 0 ){
                 operator = le_select_op ; 
            }
            else if (strcmp(node->operator , "AND")== 0 ){
                 operator = and_op ; 
            }
            else if (strcmp(node->operator , "OR")== 0 ){
                 operator = or_op ; 
            }
            else if (strcmp(node->operator , "IS NULL")== 0 ){
                 operator = is_null ; 
            }
            else if(strcmp(node->operator , "IS NOT NULL")== 0 ){
                 operator = is_not_null ; 
            }
            else if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                if (node->acc_reg == -1 ){
                    node->acc_reg = c->register_counter++   ; 
                    emit(c ,aggregate_init ,node->acc_reg , -1 , -1 , NULL  ) ; 
                }
                aggregate_select(c , node ) ; 
                if (final == true ){
                    emit(c ,aggregate_final ,node->acc_reg , -1 , node->acc_reg  , NULL  ) ; 
                    emit(c ,aggregate_reset , node->acc_reg  , -1 , -1 , NULL ) ; 
                }
            }
            else { 
                return reg  ; 
            }
            int num = col_name_to_int_main( node->ob_name , c->select   ) ; 
            int cursor = c->cursor_num ; 


            if (node->right == NULL && node->left == NULL  ){
                if (operator != is_null  && operator != is_not_null ){
                    int reg_left = c->register_counter++ ; 
                    if (1){
                        if (node->ob_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_left , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_left , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }

                    int reg_right =  c->register_counter++ ;    
                    if (1){
                        if (node->extra_ob_name != NULL ){
                            int extra_num = col_name_to_int_main( node->extra_ob_name , c->select   ) ; 
                            emit(c , column_op ,cursor , extra_num , reg_right  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_left ,reg_right , reg , -1 , NULL ) ;    
            }
            else {
                int reg_temp = c->register_counter++  ; 
                    if (1){
                        if (node->ob_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }               
            }


            else if (node->right != NULL && node->left == NULL ) {
            if (operator != is_null  && operator != is_not_null ){
                int reg_right = func(c , node->right ,final  ) ; 
                int reg_left =  c->register_counter++ ;  
                        if (1){
                            if (node->ob_name != NULL ){
                                emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_left , NULL  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_left , N-1ULL  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){p
                                    emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left, reg_right , reg , -1 , NULL ) ;   
            }
            else  {
                    int reg_temp = orderby_func(c , node->right ,final ) ; 
                    if (1){
                        if (node->ob_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_temp  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }    

            }

            else if(node->left != NULL && node->right == NULL ){
            if (operator != is_null  && operator != is_not_null ){
                int reg_left = orderby_func(c , node->left,final  ) ; 
                int reg_right =  c->register_counter++ ;  
                        if (1){
                            if (node->ob_name != NULL ){
                                emit(c , column_op ,cursor , num , reg_right  , NULL  ) ;  
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right  , reg , -1 , NULL ) ;  
            }
            else  {
                    int reg_temp = orderby_func(c , node->left ,final ) ; 
                    if (1){
                        if (node->ob_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_temp  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }
           }
            else { 
                int reg_right = orderby_func(c , node->right ,final  ) ; 
                int reg_left = orderby_func(c , node->left,final  ) ; 
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right , reg , -1 , NULL ) ;  
            }
        }

        return reg ; 
    }
    char * operand_thing(char * name) {
        int i = 0;
        while (name[i] != '.' && name[i] != '\0') {
            i= i + 1 ;
        }
        if (name[i] == '\0') {
            return NULL;
        }
        i= i + 1 ;
        int len = strlen(name + i);
        char * col_name = malloc(len + 1);
        int j = 0 ; 
        int k = 0 ; 
        for ( j = i; j < len; j++) {
            col_name[k] = name[j];
            k++ ; 
        }
        col_name[len] = '\0';
        return col_name;
    }

    char * table_thing(char * name) {
        int i = 0;
        while (name[i] != '.' && name[i] != '\0') {
            i++;
        }
        char * col_name = malloc(i + 1);
        for (int j = 0; j < i; j++) {
            col_name[j] = name[j];
        }
        col_name[i] = '\0';

        return col_name;
    }


    int get_the_join_cursor(compiler * c  , char * operator){
        int tb_num =  table_num(c , operator  ) ; 
        for ( int i  = 0 ; i < c->select->join[c->select->join_counter]->join_table_counter ; i++ ){
            if (c->select->join[c->select->join_counter]->join_select_unique_table[i] == tb_num){
                return i ; 
            }
        }
        return -1 ; 
    }

    int join_func(compiler *c , select_select_info * node  ){
        int temp = c->register_counter ; 
        int reg = join_func_main(c , node) ; 
        c->register_counter = temp ; 
        return reg ; 
    }

    int join_func_main(compiler *c , select_select_info * node   ){
        int reg  = c->register_counter   ; 
        int operator ; 
        if (node->operator != NULL  ) {
            if (strcmp(node->operator , "+")== 0 ){
                operator = add_op ; 
            }
            else if (strcmp(node->operator , "-")== 0 ){
                operator = subs_op ; 
            }
            else if  (strcmp(node->operator , "*")== 0 ){
                operator = mul_op ; 
            }   
            else if  (strcmp(node->operator , "/")== 0 ){
                operator = divide_op ; 
            }
            else if (strcmp(node->operator , "IS NULL")== 0 ){
                 operator = is_null ; 
            }
            else if(strcmp(node->operator , "IS NOT NULL")== 0 ){
                 operator = is_not_null ; 
            }
            else { 
                return reg  ; 
            }
            int num = col_name_to_int_main( operand_thing(node->col_name), c->select   ) ; 

            int cursor = get_the_join_cursor(c , table_thing(node->col_name) ) ; 


            if (node->right == NULL && node->left == NULL  ){
                if (operator != is_null  && operator != is_not_null ){
                    int reg_left = c->register_counter++ ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_left , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_left , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }

                    int reg_right =  c->register_counter++ ;    
                    if (1){
                        if (node->extra_col != NULL ){
                            int extra_num = col_name_to_int_main( operand_thing(node->extra_col), c->select   ) ; 
                            int extra_cursor = get_the_join_cursor(c , table_thing(node->extra_col) ) ;  
                            emit(c , column_op ,extra_cursor , extra_num , reg_right  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_left ,reg_right , reg , -1 , NULL ) ;    
            }
            else {
                int reg_temp = c->register_counter++  ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    
                }               
            }

            else if (node->right != NULL && node->left == NULL ) {
            if (operator != is_null  && operator != is_not_null ){
                int reg_right = func(c , node->right   ) ; 
                int reg_left =  c->register_counter++ ;  
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , column_op ,cursor , num , reg_left  , NULL  ) ;  
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_left , NULL  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_left , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_left , N-1ULL  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){p
                                    emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left, reg_right , reg , -1 , NULL ) ;   
            }
            else  {
                    int reg_temp = func(c , node->right  ) ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_temp  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }    

            }

            else if(node->left != NULL && node->right == NULL ){
            if (operator != is_null  && operator != is_not_null ){
                int reg_left = func(c , node->left  ) ; 
                int reg_right =  c->register_counter++ ;  
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , column_op ,cursor , num , reg_right  , NULL  ) ;  
                            }
                            else {
                                if (node->num_value != NULL ){
                                    emit(c , integer_op , *node->num_value , reg_right , -1  , NULL  ) ;   
                                }
                                else if (node->char_value != NULL ){
                                    emit(c , string_op ,-1 , reg_right , -1  , (void*)node->char_value   ) ;   
                                }
                                else if (node->float_val != NULL ){
                                    emit(c , real_op , -1, reg_right , -1  , (void*)node->float_val   ) ;   
                                }
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_right , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right  , reg , -1 , NULL ) ;  
            }
            else  {
                    int reg_temp = func(c , node->left  ) ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , column_op ,cursor , num , reg_temp  , NULL  ) ;  
                        }
                        else {
                            if (node->num_value != NULL ){
                                emit(c , integer_op , *node->num_value , reg_temp , -1  , NULL  ) ;   
                            }
                            else if (node->char_value != NULL ){
                                emit(c , string_op ,-1 , reg_temp , -1  , (void*)node->char_value   ) ;   
                            }
                            else if (node->float_val != NULL ){
                                emit(c , real_op , -1, reg_temp , -1  , (void*)node->float_val   ) ;   
                            }
                            else if (node->blob != NULL ){
                                emit(c , blob_op ,-1 , reg_temp , -1  , (void*)node->blob   ) ;   
                            }
                        }
                    }
                    reg = c->register_counter++  ; 
                    emit(c , operator ,reg_temp ,-1 , reg , -1 , NULL ) ;    

            }
           }
            else { 
                int reg_right = func(c , node->right   ) ; 
                int reg_left = func(c , node->left  ) ; 
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right , reg , -1 , NULL ) ;  
            }
        }

        return reg ; 
    }
}





