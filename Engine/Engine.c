engine{
    #include <limits.h>

    typedef struct instruction{
        compiler * c ;
        int op_code ;
        int p1 ; 
        int p2 ;
        int p3 ;
        void * p4  ;
    }

    typedef struct compiler{
        type * typ ; 
        int count ; 
        int capacity ; 
        int register_counter ; 
        int sorter_cursor ; 
        int register_start ; 
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

    typedef struct select_from_info{
        char * table_name ; 
        char * operator ; 
        char * as  ; 
        select_from_info *left ; 
        select_from_info * right ; 
        select_select_info * on ; 
    }

    typedef struct select_info{
        select_select_info *sel[300] ;
        int col_counter ;  
        select_from_info *from[300] ; 
        int tables_counter ; 
        select_from_info *where ; 
        select_select_info *groupby[300] ;
        int groupby_counter ; 
        int * gb_select_unique[300] ; 
        int * hash[300] ; 
        int sel_uni_counter ; 
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



    void from_parser_to_struct(compiler * c  , tree * select ){
        select_from_info **from  = c->select.from ; 
        int temp = 0   ; 
        while (temp < select->num && strcmp(select->children[temp]->comp , "FROM") != 0 ){
            temp++ ; 
        }
        select = select->children[temp] ; 
        int i = 0 ; 
        int num = 0 ; 
            while (  i < select->num && strcmp(select->children[i]->comp , "WHERE") != 0 ){
                if (strcmp(select->children[i]->comp , "ON") != 0 ){
                    from[num] = malloc(sizeof(select_from_info)) ; 
                    from[num]->table_name = select->children[i]->comp ; 
                    from[num]->operator = NULL ; 
                    from[num]->left = NULL ; 
                    from[num]->right = NULL ; 
                    if (select->as != NULL ){
                        from[num]->as = select->as ; 
                    }
                    num++ ; 
                }
                else { 
                    select_select_info * tempo = malloc(sizeof(select_select_info)) ; 
                    tempo = expre(tempo  , c , select->children[i]->children[0]  ) ; 
                    if (select->children[i]->as != NULL ){
                        tempo->as = select->children[i]->as; 
                    }
                   from[num]->on =   tempo  ; 
                    num++ ; 
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
                            table * temp = lookup_table(table_list , c->select.from[k] ) ; 
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

            }
            else if (strcmp(select->children[i]->comp  , "CASE") == 0 ){

            }
            else if (strcmp(select->children[i]->comp  , "GROUP BY") == 0 ){
                tree * groupby = c->select->children[i]->comp ; 
                int k = 0 ; 
                select_select_info *gb =  c->select->groupby ; 
                    while (k < groupby->num && strcmp(groupby->children[k]->comp , "HAVING") != 0  ){
                    if (strcmp(groupby->children[k]->comp, "+") == 0 || strcmp(groupby->children[k]->comp, "-") == 0 ||strcmp(groupby->children[k]->comp, "*") == 0 || strcmp(groupby->children[k]->comp, "/") == 0 || strcmp(groupby->children[k]->comp, "=") == 0 || strcmp(groupby->children[k]->comp, "!=") == 0 || strcmp(groupby->children[k]->comp, ">") == 0 || strcmp(groupby->children[k]->comp, ">=") == 0 || strcmp(groupby->children[k]->comp, "<") == 0 || strcmp(groupby->children[k]->comp, "<=") == 0 || strcmp(groupby->children[k]->comp, "GROUP_CONCAT") == 0 || strcmp(groupby->children[k]->comp, "MAX") == 0 || strcmp(groupby->children[k]->comp, "MIN") == 0 || strcmp(groupby->children[k]->comp, "COUNT") == 0 || strcmp(groupby->children[k]->comp, "AVG") == 0 || strcmp(groupby->children[k]->comp, "SUM") == 0) {
                        gb[c->select.groupby_counter] = malloc(sizeof(select_select_info));
                        gb[c->select.groupby_counter] =
                        expre(gb[c->select.groupby_counter], c, groupby->children[k]);
                        if (select->as != NULL) {
                            gb[c->select.groupby_counter]->as = select->as;
                        }
                        c->select.groupby_counter++;
                    }
                    else {
                        if (col_name_to_int_main(groupby->children[k]->comp, c->select) != -1) {
                            gb[c->select.groupby_counter] = malloc(sizeof(select_select_info));
                            gb[c->select.groupby_counter]->col_name = groupby->children[k]->comp;
                            gb[c->select.groupby_counter]->operator = NULL;
                            gb[c->select.groupby_counter]->left = NULL;
                            gb[c->select.groupby_counter]->right = NULL;
                            if (select->as != NULL) {
                                gb[c->select.groupby_counter]->as = select->as;
                            }
                            c->select.groupby_counter++;
                        }
                        else {
                            int check = data_type_check(groupby->children[k]->comp);
                            if (check == 0) {
                                gb[c->select.groupby_counter] = malloc(sizeof(select_select_info));
                                gb[c->select.groupby_counter]->col_name = NULL;
                                gb[c->select.groupby_counter]->operator = NULL;
                                gb[c->select.groupby_counter]->left = NULL;
                                gb[c->select.groupby_counter]->right = NULL;
                                gb[c->select.groupby_counter]->num_value = atoi(groupby->children[k]->comp);
                                c->select.groupby_counter++;
                            }
                            else if (check == 1) {

                                gb[c->select.groupby_counter] = malloc(sizeof(select_select_info));
                                gb[c->select.groupby_counter]->col_name = NULL;
                                gb[c->select.groupby_counter]->operator = NULL;
                                gb[c->select.groupby_counter]->left = NULL;
                                gb[c->select.groupby_counter]->right = NULL;
                                gb[c->select.groupby_counter]->float_val = (float)atof(groupby->children[k]->comp);
                                c->select.groupby_counter++;
                            }
                            else if (check == 2) {

                                gb[c->select.groupby_counter] = malloc(sizeof(select_select_info));
                                gb[c->select.groupby_counter]->col_name = NULL;
                                gb[c->select.groupby_counter]->operator = NULL;
                                gb[c->select.groupby_counter]->left = NULL;
                                gb[c->select.groupby_counter]->right = NULL;
                                gb[c->select.groupby_counter]->blob =  groupby->children[k]->comp;
                                c->select.groupby_counter++;
                            }
                            else {

                                gb[c->select.groupby_counter] = malloc(sizeof(select_select_info));
                                gb[c->select.groupby_counter]->col_name = NULL;
                                gb[c->select.groupby_counter]->operator = NULL;
                                gb[c->select.groupby_counter]->left = NULL;
                                gb[c->select.groupby_counter]->right = NULL;
                                gb[c->select.groupby_counter]->char_value = groupby->children[k]->comp;
                                c->select.groupby_counter++;
                            }
                        }
                    }

                    k++;
                }
                else if (strcmp(groupby->children[k]->comp , "HAVING") == 0  ){
                    
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


    // the funtion below is wrong the entire of the structure is changed need to do it cool 

    int col_name_to_int_main( char * column_name , select_info *sf  ){
        int num = -1 ; 
        for ( int i = 0 ; i < sf->tables_counter ; i++ ){
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
        emit(c , rewind_cursor , cursor , -1 , -1 , -1 , NULL  ) ; 
        int register_num = c->register_counter++ ; 
        c->register_start = register_num ; 
        int loop_addr = c->count ; 
        emit(c , eq_op , where_func(c ,c->select->where ) , -1  , INT_MAX , "BINARY" ) ; 
        if (c->select.groupby_counter == 0 ){
            for ( int i = 0 ; i < c->select.col_counter ; i++  ){
                int num = col_name_to_int_main( c->select.sel[i].col_name , c->select   ) ; 
                    if (c->select.sel[i].operator == NULL ){
                        register_num = c->register_counter++ ; 
                        if (1){
                            select_select_info *node = c->select.sel[i] ; 
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
                             int not_needed =  func(c ,c->select.sel[i] ) ; 
                        }
                    }
            }
        }

        else {
            emit(c , sorter_open , c->sorter_cursor ,c->select->groupby_counter , -1 , { col_name_to_int_main(c->select->groupby[sel_uni_counter]->col_name   , c->select)} ) ; 
            get_all_select_stuff(c) ; 
            get_all_hash_covered_gb(c) ; 
            int loop_addr_gb = c->count ; 
            sort_groupby(c) ; 
            emit(c , next_cursor , cursor , loop_addr_gb   , -1 , NULL ) ; 
            emit(c , rewind_cursor , cursor , -1 , -1 , -1 , NULL  ) ; 
            emit(c , sorter_sort , c->sorter_cursor , -1 , -1 , NULL ) ; 
            bool first  = true ; 
            emit(c , sorter_next , c->sorter_cursor , -1 , -1 , NULL ) ; 
            if (first == false ){
                emit(c , sorter_data , c->sorter_cursor , INT_MAX - 2    , -1 , NULL ) ; 
                emit(c ,gb_sorter_data , INT_MAX - 2    , c->select->sel_uni_counter , INT_MAX - 2  , NULL    ) ; 
                emit(c , eq_op , INT_MAX - 2   , -1 ,  INT_MAX - 1  , NULL  ) ; 
                int addr = c->count ; 

                for ( int i = 0 ; i < c->select.groupby_counter ; i++  ){
                    int num = col_name_to_int_main( c->select.groupby[i].col_name , c->select   ) ; 
                        if (c->select.groupby[i].operator == NULL ){
                            register_num = c->register_counter++ ; 
                            if (1){
                                select_select_info *node = c->select.groupby[i] ; 
                                if (node->col_name != NULL ){
                                    if (num != -1 ){
                                    emit(c , gb_specific_column_op , INT_MAX - 1 , num , register_num  , NULL  ) ;  
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
                                int not_needed =  func(c ,c->select.groupby[i] ) ; 
                            }
                        }
                }


                emit(c , copy_op , INT_MAX - 2  , INT_MAX -1  , -1 , NULL ) ; 
            }
            if (first == true ){
                emit(c , sorter_data , c->sorter_cursor , INT_MAX -1   , -1 , NULL ) ; 
                emit(c , gb_sorter_data , INT_MAX - 1  , c->select->sel_uni_counter , INT_MAX - 1 , NULL    ) ; 
            }

            first = false ; 

         }


        emit(c , result_row ,c->register_start , c->register_start + c->register_counter , -1  , -1 , NULL ) ; 
        c->typ[loop_addr].p2 = c->count ; 
        emit(c , next_cursor , cursor , loop_addr   , -1 , NULL ) ; 
        emit(c, close_cursor_op , cursor, -1, -1, -1, NULL);
        emit(c, halt, -1, -1, -1, -1, NULL);
    }

    // one more boring stuff simply see liek here in the where na we have to check wheter the thing we have is 0 or 1 true or false if it is false you need to do the next command execute so for that one once you do the next command so now the thing is na we have the eq_op bytecode for the thing which like checks if the thing is  true or flase  then it like jumps to the next part the issue we dont know where the next_op thing will come in the execution so we simply put it as -1 and then we just updat ething thing when we find it simple as that 
    // okay one of the most insane boring thing which happens here is see man like the loop occurs in the bytecodes itself so when we like put the register_counter like see we did the thing and as soo nas we hit the next_op it calls the bytecoders which we passed on earleir the earleir one okay only that gets called we are not calling anything in the compile_seelct getting ti it is complelty different thing got it 



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
                    select_select_info *temp = malloc(sizeof(select_select_info)) ; 
                    temp = c->select->sel[i] ; 
                    if (temp->operator != NULL ){
                        get_the_data_tree(temp , c->select) ; 
                    }
                }
        }
        }
        return  ; 
    }

    void get_all_hash_covered_gb(compiler * c ){
        int i = 0 ; 
        int num ; 
        while ( i < c->select->groupby_counter){
            num = col_name_to_int_main( temp->col_name , c->select  )   ; 
            if (num != -1 ){
                if (c->select->groupby[i]->operator == NULL ){
                    if ( c->select->hash[num] != num  ){
                        c->select->hash[num] = num  ; 
                    }
                }
                else { 
                    get_the_tree_hash(c->select->groupby[i] , c->select) ; 
                }
            }
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
                        sf->hash[extra_num] != extra_num  ; 
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
            get_the_data_tree(temp->left , sf) ; 
        }
        else if (temp->right != NULL ){
            get_the_data_tree(temp->right , sf) ; 
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
                if ( sf->hash[extra_num] != extra_num ){
                    sf->hash[extra_num] = extra_num  ; 
                    sf->gb_select_unique[sf->sel_uni_counter++ ] = extra_num ; 
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
        else if (temp->right != NULL ){
            get_the_data_tree(temp->right , sf) ; 
        }

        return ; 
    }

    void sort_groupby(compiler * c  ){
        int i = 0 ; 
        int start = c->register_start + c->register_counter ; 
        int cur = start ; 
        int cursor_sort = c->sorter_cursor++ ; 
        int norm_cursor = c->cursor_num++  ; 
        while ( i < c->select->sel_uni_counter){
            emit(c , column_op ,norm_cursor , c->select->gb_select_unique[i] , cur  , NULL  ) ;    
            i++ ; 
            cur++ ; 
        }
        i = 0 ; 
        while ( i < c->select->groupby_counter){
            int num = col_name_to_int_main( c->select->groupby[i]->col_name , c->select   )   ; 
            if (num != -1   ){
                if (c->select->groupby[i]->operator != NULL ){
                    int temp = c->register_counter ; 
                    c->register_counter = cur ; 
                    int node =  func(c ,c->select->groupby[i]  ) ;
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
            reg = func(c, node->left, cursor);
        }
        else {                         
            reg = -1;
        }
        emit(c , aggregate_step , node->acc_reg , reg , NULL , operation) ; 
    }


    int where_func(compiler *c , select_select_info * node ){
        int first_reg = c->register_counter ; 
        int ans = func(c , node) ; 
        emit(c , integer_op , 0 , INT_MAX , -1 , NULL ) ; 
        c->register_counter = first_reg ; 
        return ans ;  
    }



    int  func(compiler *c , select_select_info * node , bool final  ){
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
            if (strcmp(node->operator , "AND")== 0 ){
                 operator = and_op ; 
            }
            if (strcmp(node->operator , "OR")== 0 ){
                 operator = or_op ; 
            }
            if (strcmp(node->operator , "IS NULL")== 0 ){
                 operator = is_null ; 
            }
            if (strcmp(node->operator , "IS NOT NULL")== 0 ){
                 operator = is_not_null ; 
            }
            if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                if (node->acc_reg == -1 ){
                    node->acc_reg = c->register_counter++   ; 
                    emit(c ,aggregate_init ,node->acc_reg , -1 , -1 , NULL  ) ; 
                }
                aggregate_select(c , node ) ; 
                if (final == true ){
                    emit(c ,aggregate_final ,node->acc_reg , -1 , node->acc_reg  , NULL  ) ; 
                }
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
            if (strcmp(node->operator , "AND")== 0 ){
                 operator = and_op ; 
            }
            if (strcmp(node->operator , "OR")== 0 ){
                 operator = or_op ; 
            }
            if (strcmp(node->operator , "IS NULL")== 0 ){
                 operator = is_null ; 
            }
            if (strcmp(node->operator , "IS NOT NULL")== 0 ){
                 operator = is_not_null ; 
            }
            if (strcmp(node->operator , "GROUP_CONCAT")== 0 || strcmp(node->operator , "MAX") == 0   || strcmp(node->operator , "MIN") == 0 || strcmp(node->operator , "COUNT") == 0 || strcmp(node->operator , "AVG") == 0 || strcmp(node->operator , "SUM") == 0     ){
                if (node->acc_reg == -1 ){
                    node->acc_reg = c->register_counter++   ; 
                    emit(c ,aggregate_init ,node->acc_reg , -1 , -1 , NULL  ) ; 
                }
                aggregate_select(c , node ) ; 
                if (final == true ){
                    emit(c ,aggregate_final ,node->acc_reg , -1 , node->acc_reg  , NULL  ) ; 
                }
            }
            int num = col_name_to_int_main( node->col_name , c->select   ) ; 
            int cursor = c->cursor_num ; 


            if (node->right == NULL && node->left == NULL  ){
                if (operator != is_null  && operator != is_not_null ){
                    int reg_left = c->register_counter++ ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , gb_specific_column_op , INT_MAX - 1 , num , reg_left  , NULL  ) ;  
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
                            emit(c , gb_specific_column_op , INT_MAX - 1 , extra_num , reg_left  , NULL  ) ;   
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
                                emit(c , gb_specific_column_op , INT_MAX - 1 , num , reg_left  , NULL  ) ; 
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
                                emit(c , gb_specific_column_op , INT_MAX - 1 , num , reg_left  , NULL  ) ; 
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
                    int reg_temp =  group_by_func(c , node->right  , final ) ; 
                    if (1){
                        if (node->col_name != NULL ){
                            emit(c , gb_specific_column_op , INT_MAX - 1 , num , reg_temp  , NULL  ) ; 
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
        }

            else if(node->left != NULL && node->right == NULL ){
            if (operator != is_null  && operator != is_not_null ){
                int reg_left =  group_by_func(c , node->left  , final ) ; 
                int reg_right =  c->register_counter++ ;  
                        if (1){
                            if (node->col_name != NULL ){
                                emit(c , gb_specific_column_op , INT_MAX - 1 , num , reg_right  , NULL  ) ; 
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
                            emit(c , gb_specific_column_op , INT_MAX - 1 , num , reg_temp  , NULL  ) ; 
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
            else { 
                int reg_right =  group_by_func(c , node->right  , final ) ; 
                int reg_left =  group_by_func(c , node->left  , final ) ; 
                reg = c->register_counter++  ; 
                emit(c , operator ,reg_left , reg_right , reg , -1 , NULL ) ;  
            }
        }

        return reg ; 
    }

}





