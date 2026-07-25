engine{
    tree *find_node(tree * jhad , char *word  , tree *ans , int first ){
        if ( jhad == NULL ){
            return NULL ; 
        }
        else if ( ans != NULL ){
            return ans ; 
        }
        if (strcmp(word , "SELECT") == 0 && first == 1  ){
            if ( strcmp(jhad->comp, word) == 0  ){
                ans  = jhad ; 
                return ans ; 
            }
        }

        int i = 0 ; 
        while (i < jhad->num){
            if (word != "SELECT" && strcmp(jhad->children[i]->comp , "SELECT" ) ==0  ){
                find_node(jhad->children[i] , word  , ans , 1  ) ; 
            }
        }
        return ans ; 
    }


    typedef struct find_the {
        struct tree* groupby;
        struct tree* orderby;
        struct tree* limit;
        struct tree* from;
        struct tree* having;
        struct tree* offset;
        struct tree* select;
        struct tree* where;
        struct tree* unio;
        struct tree* except;
        struct tree* intersect;
        struct tree* distinct;
        struct tree* join;
        struct tree* innerjoin;
        struct tree* leftjoin;
        struct tree* rightjoin;
        struct tree* crossjoin;
        struct tree* fullouterjoin;
        struct tree* case_expre ;
        struct tree* unioall;
        struct tree* exceptall;
        struct tree* intersectall;
    } find_the;



    typedef  find_the find  ; 
    struct tree *node = NULL;

    find* find_func(tree * jhad){
        find*temp ; 
        temp->groupby = find_node(jhad , "GROUP BY" , NULL) ; 
        temp->orderby = find_node(jhad, "ORDER BY", NULL);
        temp->limit = find_node(jhad, "LIMIT", NULL);
        temp->from = find_node(jhad, "FROM", NULL);
        temp->having = find_node(jhad, "HAVING", NULL);
        temp->offset = find_node(jhad, "OFFSET", NULL);
        temp->select = find_node(jhad, "SELECT", NULL);
        temp->where = find_node(jhad, "WHERE", NULL);
        temp->unio = find_node(jhad, "UNION", NULL);
        temp->except = find_node(jhad, "EXCEPT", NULL);
        temp->intersect = find_node(jhad, "INTERSECT", NULL);
        temp->distinct = find_node(jhad, "DISTINCT", NULL);
        temp->join = find_node(jhad, "JOIN", NULL);
        temp->innerjoin = find_node(jhad, "INNER JOIN", NULL);
        temp->leftjoin = find_node(jhad, "LEFT JOIN", NULL);
        temp->rightjoin = find_node(jhad, "RIGHT JOIN", NULL);
        temp->crossjoin = find_node(jhad, "CROSS JOIN", NULL);
        temp->fullouterjoin = find_node(jhad, "FULL OUTER JOIN", NULL);
        temp->case = find_node(jhad, "CASE", NULL);
        temp->unioall = find_node(jhad, "UNION ALL", NULL);
        temp->exceptall = find_node(jhad, "EXCEPT ALL", NULL);
        temp->intersectall = find_node(jhad, "INTERSECT ALL", NULL);
        return temp ; 
    }

    typedef struct compiler{
        type * typ ; 
        int count ; 
        int capacity ; 
        int register_counter ; 
        int register_start ; 
        int cursor_num ; 
        select_info select ; 
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

    typedef struct select_from_info{
        char * table_name[300] ; 
        int tables_counter ; 
        char * operator ; 
        char * as  ; 
        select_from_info *left ; 
        select_from_info * right ; 
    }

    typedef struct select_info{
        select_select_info *sel[300] ;
        int col_counter ;  
        select_from_info *from ; 
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

    int col_name_to_int( char * column_name , table * t ){
        for (int i = 0 ; i < t->num_of_columns ; i++ ){
           if ( strcmp(t->col[i].name , column_name ) == 0 ){
              return i ; 
           }
        }
        return -1  ; 
    }

    int col_name_to_int_main( char * column_name , select_from_info from  ){
        int num = -1 ; 
        for ( int i = 0 ; i < from->tables_counter ; i++ ){
            int number = col_name_to_int(column_name ,from->table_name[i] ) ; 
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

    table * lookup_table( tables_list * tab , char * table_name ){
        for ( int i = 0 ; i < tab->num_of_tables ; i++ ){
           if ( strcmp( tab[i]->name  , table_name  ) == 0 ) { 
                return tab[i] ; 
           }
        }
        return NULL ; 
    }

    void compile_select (compiler *c ){
        emit(c , begin_op  , -1 , -1 , -1 , NULL ) ; 
        int cursor = c->cursor_num++ ; 
        emit(c , open_read_op , cursor , sql_master->page_num ,  -1 , -1 , NULL    ) ; 
        
        for ( int i = 0 ; i < c->select.col_counter ; i++  ){
            select_select_info *node = c->select.sel[i].col_name ; 
            if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                node->acc_reg = c->register_counter++   ; 
                emit(c ,aggregate_init ,node->acc_reg , -1 , -1 , NULL  ) ; 
            }
        }

        emit(c , rewind_cursor , cursor , -1 , -1 , -1 , NULL  ) ; 
        int register_num = c->register_counter++ ; 
        c->register_start = register_num ; 
        int loop_addr = c->count ; 
            for ( int i = 0 ; i < c->select.col_counter ; i++  ){
                int num = col_name_to_int_main( c->select.sel[i].col_name , c->select.from   ) ; 
                if (num != -1 ){
                    if (c->select.sel[i].operator == NULL ){
                        register_num = c->register_counter++ ; 
                        if (1){
                            select_select_info *node = c->select.sel[i] ; 
                            if (node->col_name != NULL ){
                                emit(c , column_op ,cursor , num , register_num  , NULL  ) ;  
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
                        int not_needed =  func(c ,c->select.sel[i] ) ; 
                    }
                }
            }
        emit(c , result_row ,c->register_start , c->register_start + c->register_counter , -1  , -1 , NULL ) ; 
        emit(c , next_cursor , cursor , loop_addr   , -1 , NULL ) ; 
        for ( int i = 0 ; i < c->select.col_counter ; i++  ){
            select_select_info *node = c->select.sel[i].col_name ; 
            if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                emit(c ,aggregate_final ,node->acc_reg , -1 , node->acc_reg  , NULL  ) ; 
            }
        }
        emit(c, close_cursor_op , cursor, -1, -1, -1, NULL);
        emit(c, halt, -1, -1, -1, -1, NULL);
    }


// okay one of the most insane boring thing which happens here is see man like the loop occurs in the bytecodes itself so when we like put the register_counter like see we did the thing and as soo nas we hit the next_op it calls the bytecoders which we passed on earleir the earleir one okay only that gets called we are not calling anything in the compile_seelct getting ti it is complelty different thing got it 
    


    void aggregate_select(compiler *c , select_select_info * node  ){
        void * operation = node->operator ; 
        int reg ; 
        if (node->col_name != NULL) {             
            int num = col_name_to_int_main(node->col_name, c->select.from);
            reg = c->register_counter++;
            emit(c, column_op, cursor, num, reg, NULL);
        }
        else if (node->left != NULL) {             
            reg = func(c, node->left, cursor);
        }
        else {                         
            reg = -1;
        }
        emit(c , aggregate_step , node->acc_reg , reg , NULL , operation) ; 
    }


    int  func(compiler *c , select_select_info * node ){
        int reg   ; 
        int operator ; 
        if (node->operator != NULL  ) {
            if (strcmp(node->operator , "+")== 0 ){
                operator = add_op ; 
            }
            if (strcmp(node->operator , "-")== 0 ){
                operator = subs_op ; 
            }
            if (strcmp(node->operator , "*")== 0 ){
                operator = mul_op ; 
            }   
            if (strcmp(node->operator , "/")== 0 ){
                operator = divide_op ; 
            }
            if (strcmp(node->operator , "=")== 0 ){
                operator = eq_select_op ; 
            }
            if (strcmp(node->operator , "!=")== 0 ){
                 operator = ne_select_op ; 
            }
            if (strcmp(node->operator , ">")== 0 ){
                 operator = gt_select_op ; 
            }
            if (strcmp(node->operator , ">=")== 0 ){
                    operator = ge_select_op ; 
            }
            if (strcmp(node->operator , "<")== 0 ){
                 operator = lt_select_op ; 
            }
            if (strcmp(node->operator , "<=")== 0 ){
                 operator = le_select_op ; 
            }
            if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                aggregate_select(c , node ) ; 
            }
            int num = col_name_to_int_main( c->select.sel[i].col_name , c->select.from   ) ; 
            int cursor = c->cursor_num ; 


            if (node->right == NULL && node->left == NULL  ){
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
                emit(c , operator ,reg_left ,reg_right , reg , -1 , NULL ) ;                   
            }


            else if (node->right != NULL && node->left == NULL ) {
                int reg_right = func(c , node->right ) ; 
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
                                else if (node->blob != NULL ){
                                    emit(c , blob_op ,-1 , reg_left , -1  , (void*)node->blob   ) ;   
                                }
                            }
                        }
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left, reg_right , reg , -1 , NULL ) ;   
            }
            else if(node->left != NULL && node->right == NULL ){
                int reg_left = func(c , node->left ) ; 
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
            else { 
                int reg_right = func(c , node->right ) ; 
                int reg_left = func(c , node->left ) ; 
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right , reg , -1 , NULL ) ;  
            }
        }

        return reg ; 
    }

}