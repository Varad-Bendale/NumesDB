engine{
    

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
        char * table_name ; 
        char * operator ; 
        char * as  ; 
        select_from_info *left ; 
        select_from_info * right ; 
    }

    typedef struct select_info{
        select_select_info *sel[300] ;
        int col_counter ;  
        select_from_info *from[300] ; 
        int tables_counter ; 
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

    select_select_info expre(select_select_info *ans ,    compiler *c , tree * temp){
        int i = 0 ; 
        while (i < temp->num){
            if (strcmp(temp->children[i].comp , "+")== 0 || strcmp(temp->children[i].comp, "-")== 0 || strcmp(temp->children[i].comp, "*")== 0 || strcmp(temp->children[i].comp, "/")== 0 || strcmp(temp->children[i].comp , "=")== 0 || strcmp(temp->children[i].comp, "!=")== 0 || strcmp(temp->children[i].comp , ">")== 0 || strcmp(temp->children[i].comp , ">=")== 0 || strcmp(temp->children[i].comp , "<")== 0 || strcmp(temp->children[i].comp, "<=")== 0 || strcmp(temp->children[i].comp , "GROUP_CONCAT")== 0 || strcmp(temp->children[i].comp , "MAX") == 0 || strcmp(temp->children[i].comp , "MIN") == 0 || strcmp(temp->children[i].comp , "COUNT") == 0 || strcmp(temp->children[i].comp, "AVG") == 0 || strcmp(temp->children[i].comp , "SUM") == 0){
                   ans->operator = temp->children[i].comp;
                if (ans->left == NULL ){
                    ans->left = malloc(sizeof(select_select_info))
                    expre(ans->left, c, temp->children[i]);
                }
                else {
                    ans->right = malloc(sizeof(select_select_info))
                    expre(ans->right, c, temp->children[i]);
                }
            }
            else if (col_name_to_int_main(temp->children[i].comp, c->select.from) != -1 ){
                ans->col_name = temp->children[i].comp ;
            }
            else { 
                int check = data_type_check(temp->children[i].comp);
                if (check == 0){
                    ans->num_value = atoi(temp->children[i].comp);
                }
                else if (check == 1){
                    ans->float_val = (float)atof(temp->children[i].comp);
                }
                else if (check == 2){
                    ans->blob = temp->children[i].comp ;
                }
                else {
                    ans->char_value = temp->children[i].comp ;
                }
            }
        i++;
        }
        return ans ; 
    }


    select_from_info expre_from(select_from_info *ans , compiler *c , tree * temp){
        int i = 0 ; 
        while (i < temp->num){
            if (strcmp(temp->children[i].comp , "FULL OUTER JOIN")== 0 ||  strcmp(temp->children[i].comp , "CROSS JOIN")== 0 || strcmp(temp->children[i].comp , "AND")== 0 ||strcmp(temp->children[i].comp , "INNER JOIN")== 0 || strcmp(temp->children[i].comp , "JOIN")== 0 || strcmp(temp->children[i].comp , "LEFT JOIN")== 0 || strcmp(temp->children[i].comp , "RIGHT JOIN")== 0 || strcmp(temp->children[i].comp , "OR")== 0 || strcmp(temp->children[i].comp , "NOT")== 0 ||  strcmp(temp->children[i].comp , "+")== 0 || strcmp(temp->children[i].comp, "-")== 0 || strcmp(temp->children[i].comp, "*")== 0 || strcmp(temp->children[i].comp, "/")== 0 || strcmp(temp->children[i].comp , "=")== 0 || strcmp(temp->children[i].comp, "!=")== 0 || strcmp(temp->children[i].comp , ">")== 0 || strcmp(temp->children[i].comp , ">=")== 0 || strcmp(temp->children[i].comp , "<")== 0 || strcmp(temp->children[i].comp, "<=")== 0 || strcmp(temp->children[i].comp , "GROUP_CONCAT")== 0 || strcmp(temp->children[i].comp , "MAX") == 0 || strcmp(temp->children[i].comp , "MIN") == 0 || strcmp(temp->children[i].comp , "COUNT") == 0 || strcmp(temp->children[i].comp, "AVG") == 0 || strcmp(temp->children[i].comp , "SUM") == 0){
                   ans->operator = temp->children[i].comp;
                if (ans->left == NULL ){
                    ans->left = malloc(sizeof(select_from_info)) ; 
                    expre_from(ans->left, c, temp->children[i]);
                }
                else {
                    ans->right = malloc(sizeof(select_from_info)) ; 
                    expre_from(ans->right, c, temp->children[i]);
                }
            }
            else if (col_name_to_int_main(temp->children[i].comp, c->select.from) != -1 ){
                ans->col_name = temp->children[i].comp ;
            }
        i++;
        }
        return ans ; 
    }


    void from_parser_to_struct(compiler * c  , tree * select ){
        select_from_info **from  = c->select.from ; 
        int temp = 0   ; 
        while (temp < select->num && strcmp(select->children[temp].comp , "FROM") != 0 ){
            temp++ ; 
        }
        select = select->children[temp] ; 
        int i = 0 ; 
        int num = 0 ; 
        while ( i < select->num ){
            while (strcmp(select->children[i].comp , "WHERE") != 0 ){
                if (strcmp(select->children[i].comp , "ON") != 0 ){
                    from[num] = malloc(sizeof(select_from_info)) ; 
                    from[num]->table_name = select->children[i].comp ; 
                    from[num]->operator = NULL ; 
                    from[num]->left = NULL ; 
                    from[num]->right = NULL ; 
                    if (select->as != NULL ){
                        from[num]->as = select->as ; 
                    }
                    num++ ; 
                }
                else { 
                    from[num] = expre_from(from[num] , c , select->children[i].children[0]  ) ; 
                    if (select->children[i]->as != NULL ){
                        from[num]->as = select->children[i]->as; 
                    }
                    num++ ; 
                }
            }
            i++ ; 
        }
    }

//okay my brain got shut so yeah i am wirtijng this down here see the problem is na go to expre first asnd then check out like if the thing for like at the very end we reach then we need liek 2 operands for the ework i feel it kind of lackds do trace it out and then see whats the problem get it done here the from thing would be done as well side by side okay so yeah work from here okay 
    void select_parser_to_struct(  compiler *c , tree * select ){
        select_select_info sel = c->select.sel ; 
        int num =  0 ; 
        int i = 0 ; 
        while ( i < select->num ){
            from_parser_to_struct(c , select ) ; 
            while(strcmp(select->children[i].comp  , "FROM") == 0  ){
                if (1){
                    if (strcmp(select->children[i].comp  , "*") == 0 ){
                        for (int k = 0 ; k < c->tables_counter ; k++ ){
                            table * temp = lookup_table(table_list , c->select.from[k] ) ; 
                            for ( int j = 0 ; j < temp->num_of_columns ; j++ ){
                                sel[num] = malloc(sizeof(select_select_info)) ; 
                                sel[num]->col_name = temp->col[j].name ; 
                                sel[num]->operator = NULL ; 
                                sel[num]->left = NULL ; 
                                sel[num]->right = NULL ; 
                                if (select->as != NULL ){
                                    sel[i]->as = select->as ; 
                                }
                                num++ ; 
                            }
                        }
                    }   
                    else  {
                        if (strcmp(select->children[i].comp , "+")== 0 || strcmp(select->children[i].comp, "-")== 0 || strcmp(select->children[i].comp , "*")== 0  || strcmp(select->children[i].comp , "/")== 0 || strcmp(select->children[i].comp, "=")== 0 || strcmp(select->children[i].comp , "!=")== 0  || strcmp(select->children[i].comp, ">")== 0 ||  strcmp(select->children[i].comp , ">=")== 0  || strcmp(select->children[i].comp , "<")== 0 ||  strcmp(select->children[i].comp , "<=")== 0 || strcmp(select->children[i].comp , "GROUP_CONCAT")== 0 || strcmp(select->children[i].comp, "MAX") == 0 || strcmp(select->children[i].comp , "MIN") == 0 || strcmp(select->children[i].comp, "COUNT") == 0 || strcmp(select->children[i].comp, "AVG") == 0 || strcmp(select->children[i].comp , "SUM") == 0){
                                sel[num] = malloc(sizeof(select_select_info)) ; 
                                sel[num] = expre(sel[num] , c , select->children[i] )  ; 
                                if (select->as != NULL ){
                                    sel[i]->as = select->as ; 
                                }
                                num++ ; 
                        }
                        else { 
                            if(col_name_to_int_main(select->children[i].comp, c->select.from) != -1){
                                sel[num] = malloc(sizeof(select_select_info)) ; 
                                sel[num]->col_name = select->children[i].comp ; 
                                sel[num]->operator = NULL ; 
                                sel[num]->left = NULL ; 
                                sel[num]->right = NULL ; 
                                if (select->as != NULL ){
                                    sel[i]->as = select->as ; 
                                }
                                num++ ;  
                            }
                            else { 
                                int check = data_type_check(select->children[i].comp);
                                if (check == 0){
                                    sel[num] = malloc(sizeof(select_select_info)) ; 
                                    sel[num]->col_name =  NULL ; 
                                    sel[num]->operator = NULL ; 
                                    sel[num]->left = NULL ; 
                                    sel[num]->right = NULL ; 
                                    sel[num]->num_value = atoi(select->children[i].comp);
                                    num++ ; 
                                }
                                else if (check == 1){
                                    sel[num] = malloc(sizeof(select_select_info)) ; 
                                    sel[num]->col_name =  NULL ; 
                                    sel[num]->operator = NULL ; 
                                    sel[num]->left = NULL ; 
                                    sel[num]->right = NULL ; 
                                    sel[num++]->float_val = (float)atof(select->children[i].comp);
                                    num++ ; 

                                }
                                else if (check == 2){
                                    sel[num] = malloc(sizeof(select_select_info)) ; 
                                    sel[num]->col_name =  NULL ; 
                                    sel[num]->operator = NULL ; 
                                    sel[num]->left = NULL ; 
                                    sel[num]->right = NULL ; 
                                    sel[num++]->blob = select->children[i].comp ;
                                    num++ ; 

                                }
                                else {
                                    sel[num] = malloc(sizeof(select_select_info)) ; 
                                    sel[num]->col_name =  NULL ; 
                                    sel[num]->operator = NULL ; 
                                    sel[num]->left = NULL ; 
                                    sel[num]->right = NULL ; 
                                    sel[num++]->char_value = select->children[i].comp ;
                                    num++ ; 
                                }
                            }
                        }
                    }
                }
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