
#pragma once

#define READ_STRING_RETURN		code >> com;\
								if( code.eof() )\
									return;\
								for( int i = 0; i < com.size(); ++i )\
									if( com[i] >= 'A' && com[i] <= 'Z' )\
										com[i] += 'a' - 'A';

#define READ_STRING_BREAK		code >> com;\
								if( code.eof() )\
									break;\
								for( int i = 0; i < com.size(); ++i )\
									if( com[i] >= 'A' && com[i] <= 'Z' )\
										com[i] += 'a' - 'A';

#define READ_STRING_CONTINUE	code >> com;\
								if( code.eof() )\
									continue;\
								for( int i = 0; i < com.size(); ++i )\
									if( com[i] >= 'A' && com[i] <= 'Z' )\
										com[i] += 'a' - 'A';

#define PUSH_UINT64_TO_DATA(a__)			SetIntAt( a__, data.size() );

#define PUSH_DATA_COMMAND(a__)		data.resize( data.size() + 1 );\
									data.back() = a__;

void MyAssemblyLang::PrimitiveCompiler( const char * fileName )
{
	std::map < std::string, uint64 > labels;					// name - id
	std::map < uint64, uint64 > labelId_label;					// id - machine code pointer
	std::map < uint64, uint64 > labelPointer;					// in code pointer to place were it's used - id
	
	std::map < std::string, std::map < std::string, uint64 > > variables;				// name - pointer
	std::map < std::string, std::map < uint64, uint64 > > variableId_variable;			// id - machine code pointer
	std::map < std::string, std::map < uint64, uint64 > > variablePointer;				// in code pointer to place were it's used - id
	
	std::map < std::string, uint64 > functions;					// name - pointer
	std::map < uint64, uint64 > functionId_function;			// id - machine code pointer
	std::map < uint64, uint64 > functionPointer;				// in code pointer to place were it's used - id
	std::map < std::string, uint64 > variablesCounter;			// function name - counter
	
	std::ifstream code;
	code.open( fileName );
	if( code.good() )
	{
		//test
		/*
		{
			char data__fast_dwa = 0;
			code.read( &data__fast_dwa, 1 );
			code.seekg( 0, code.beg );
			std::cout << "\n Value = " << (int)data__fast_dwa;
			printf( "\n Press any Key\n" );
			getch();
		}
		*/
		
		std::string currentFunction = "NO_FUNCTION__";
		currentFunction = "NO_FUNCTION__";
		uint64 labelsCounter = 0, functionsCounter = 0;
		std::string com, type;
		char * fastData = new char[4096];
		while( true )
		{
			READ_STRING_BREAK;
			
			if( com[0] == '/' && com[1] == '/' )
			{
				code.getline( fastData, 4095 );
			}
			else if( com == "label" )
			{
				READ_STRING_BREAK;
				labels[com] = labelsCounter;
				++labelsCounter;
			}
			else if( com == "var" )
			{
				READ_STRING_BREAK;	// var type
				type = com;
				READ_STRING_BREAK;	// var name
				
				if( debug ) printf( "\n Variable \"%s\" in function \"%s\"", com.c_str(), currentFunction.c_str() );
				variables[currentFunction][com] = variablesCounter[currentFunction];
				
				variablesCounter[currentFunction]++;
				if( currentFunction == "NO_FUNCTION__" )
				{
					READ_STRING_BREAK;	// var value
					if( type == "string" )
					{
						while( com[com.size()-1] != '"' )
						{
							READ_STRING_BREAK
						}
					}
					else if( type == "bytes" )
					{
						READ_STRING_BREAK;	// bytes number for type "bytes"
					}
				}
			}
			else if( com == "func" )
			{
				READ_STRING_BREAK;	// var name
				currentFunction = com;
				functions[com] = functionsCounter;
				++functionsCounter;
			}
			else if( com == "endfunc" )
			{
				if( debug ) printf( "\n \"%s\" - local variables counter: %i", currentFunction.c_str(), (int)(variablesCounter[currentFunction]) );
				currentFunction = "NO_FUNCTION__";
			}
		}
		
		code.seekg( 0, code.beg );
		currentFunction = "NO_FUNCTION__";
		
		while( true )
		{
			com = "";
			READ_STRING_BREAK;
			
			if( com[0] == '/' && com[1] == '/' )
			{
				code.getline( fastData, 4095 );
			}
			else if( com == "label" )
			{
				READ_STRING_CONTINUE;
				auto it = labels.find(com);
				if( it != labels.end() )
				{
					labelId_label[it->second] = data.size();
					if( debug ) printf( "\n Label: \"%s\"  pointer: %lli ", com.c_str(), (int64)data.size() );
				}
				else
				{
					printf( "\n Undefined label name \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "var" )		/////////////////////////////
			{
				READ_STRING_CONTINUE;
				std::string varType = com;
				READ_STRING_CONTINUE;
				auto it = variables[currentFunction].find( com );
				if( it != variables[currentFunction].end() )
				{
					if( currentFunction == "NO_FUNCTION__" )
					{
						variableId_variable[currentFunction][it->second] = data.size();
						if( debug ) printf( "\n Variable: \"%s\"  pointer: %lli ", com.c_str(), (int64)data.size() );
						if( varType == "string" )
						{
							READ_STRING_CONTINUE;
							if( false )
							{
								VAR_STRING_FORMAT_ERROR:
									printf( "\n Undefined string format \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
									getchar();
									return;
							}
							else
							{
								if( com[0] == '\"' && com[com.size()-1] == '\"' )
								{
									std::string dst;
									uint64 temp, temp1, temp2;
									for( temp = 1; temp+1 < com.size(); ++temp )
									{
										if( com[temp] == '\\' )
										{
											switch( com[++temp] )
											{
											case 't':
												dst += "\t";
												break;
											case 'n':
												dst += "\n";
												break;
											case '\\':
												dst += "\\";
											case 's':
												dst += " ";
												break;
											default:
												goto VAR_STRING_FORMAT_ERROR;
											}
										}
										else
										{
											dst.push_back( com[temp] );
										}
									}
									temp1 = data.size();
									data.resize( temp1+dst.size()+1 );
									data.back() = 0;
									for( temp2 = 0; temp2 < dst.size(); ++temp2 )
									{
										data[temp1+temp2] = dst[temp2];
									}
								}
							}
						}
						else if( varType == "int" )
						{
							int64 temp = 0;
							code >> temp;
							PUSH_UINT64_TO_DATA(temp);
						}
						else if( varType == "bytes" )
						{
							byte temp;
							uint64 temp1 = 0, temp2 = 0, temp3;
							code >> temp1;
							code >> temp2;
							temp = (byte)temp1;
							temp1 = data.size();
							data.resize( temp1 + temp2 );
							for( temp3 = 0; temp3 < temp2; ++temp3 )
								data[temp1+temp3] = temp;
						}
						else
						{
							printf( "\n Undefined variable type \"%s\"  at byte: %i", varType.c_str(), (int)code.tellg() );
							getchar();
							return;
						}
					}
					else
					{
						variableId_variable[currentFunction][it->second] = variables[currentFunction][com]<<3;
						if( debug ) printf( "\n local variable: \"%s\"  pointer: %lli ", com.c_str(), (int64)(variables[currentFunction][com]<<3) );
						if( varType != "int" )
						{
							printf( "\n Undefined variable type \"%s\"  at byte: %i\n Error: local variables can be only \"int\" and can not have value", varType.c_str(), (int)code.tellg() );
							getchar();
							return;
						}
						
					}
				}
				else
				{
					printf( "\n Undefined variable name in variable declaration \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "func" )		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				READ_STRING_CONTINUE;
				currentFunction = com;
				functionId_function[functions[currentFunction]] = data.size();
				if( debug ) printf( "\n Function declared: \"%s\"  at byte (in code): %i", currentFunction.c_str(), data.size() );
				data.resize( data.size()+8 );
				SetIntAt( variablesCounter[currentFunction]<<3, data.size()-8 );
			}
			else if( com == "endfunc" )		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				currentFunction = "NO_FUNCTION__";
			}
			else if( com == "call" )		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				READ_STRING_CONTINUE;
				auto it = functions.find( com );
				if( it != functions.end() )
				{
					PUSH_DATA_COMMAND( CALL );
					functionPointer[data.size()] = it->second;
					if( debug ) printf( "\n Function used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined call argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "ret" )			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				PUSH_DATA_COMMAND( RET );
			}
			else if( com == "end" )			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				PUSH_DATA_COMMAND( END );
			}
			else if( com == "push" )
			{
				READ_STRING_CONTINUE;
				auto it = variables[currentFunction].find( (com[0]=='&'||com[0]=='*') ? com.c_str()+1 : com );
				auto it_ = variables["NO_FUNCTION__"].find( (com[0]=='&'||com[0]=='*') ? com.c_str()+1 : com );
				if( it != variables[currentFunction].end() && currentFunction != "NO_FUNCTION__" )			/////////////////////////////////////////////////////
				{
					if( com[0] == '&' )
					{
						PUSH_DATA_COMMAND( PUSHADRESSLOCAL );
					}
					else if( com[0] == '*' )
					{
						PUSH_DATA_COMMAND( PUSHADRESSVALUELOCAL );
					}
					else
					{
						PUSH_DATA_COMMAND( PUSHLOCAL );
					}
					variablePointer[currentFunction][data.size()] = it->second;
					if( debug ) printf( "\n local variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else if( it_ != variables["NO_FUNCTION__"].end() )
				{
					if( com[0] == '&' )
					{
						PUSH_DATA_COMMAND( PUSHADRESSGLOBAL );
					}
					else if( com[0] == '*' )
					{
						PUSH_DATA_COMMAND( PUSHADRESSVALUEGLOBAL );
					}
					else
					{
						PUSH_DATA_COMMAND( PUSHGLOBAL );
					}
					variablePointer["NO_FUNCTION__"][data.size()] = it_->second;
					if( debug ) printf( "\n variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it_->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					int err = 0;
					int64 temp = MyAssemblyLang::GetInt64FromString( com, err );
					if( err )
					{
						printf( "\n Undefined push argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
						getchar();
						return;
					}
					else
					{
						PUSH_DATA_COMMAND( PUSHCONST );
						PUSH_UINT64_TO_DATA(temp);
					}
				}
			}
			else if( com == "pop" )
			{
				READ_STRING_CONTINUE;
				auto it = variables[currentFunction].find( (com[0]=='*') ? com.c_str()+1 : com );
				auto it_ = variables["NO_FUNCTION__"].find( (com[0]=='*') ? com.c_str()+1 : com );
				if( it != variables[currentFunction].end() && currentFunction != "NO_FUNCTION__" )			/////////////////////////////////////////////////////
				{
					if( com[0] == '*' )
					{
						PUSH_DATA_COMMAND( POPLOCALADRESS );
					}
					else
					{
						PUSH_DATA_COMMAND( POPLOCAL );
					}
					variablePointer[currentFunction][data.size()] = it->second;
					if( debug ) printf( "\n local variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else if( it_ != variables["NO_FUNCTION__"].end() )
				{
					if( com[0] == '*' )
					{
						PUSH_DATA_COMMAND( POPGLOBALADRESS );
					}
					else
					{
						PUSH_DATA_COMMAND( POPGLOBAL );
					}
					variablePointer["NO_FUNCTION__"][data.size()] = it_->second;
					if( debug ) printf( "\n variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it_->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					int err = 0;
					int64 temp = MyAssemblyLang::GetInt64FromString( com, err );
					if( err )
					{
						printf( "\n Undefined pop argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
						getchar();
						return;
					}
					else
					{
						PUSH_DATA_COMMAND( PUSHCONST );
						PUSH_UINT64_TO_DATA(temp);
					}
				}
			}
			else if( com == "allocmem" )
			{
				PUSH_DATA_COMMAND( ALLOCATEMEMORY );
			}
			else if( com == "freemem" )
			{
				PUSH_DATA_COMMAND( FREEMEMORY );
			}
			else if( com == "reservemem" )
			{
				PUSH_DATA_COMMAND( RESERVEMEMORY );
			}
			else if( com == "freereservedmem" )
			{
				PUSH_DATA_COMMAND( FREERESERVEDMEMORY );
			}
			else if( com == "jump" )
			{
				READ_STRING_CONTINUE;
				auto it = labels.find( com );
				if( it != labels.end() )
				{
					PUSH_DATA_COMMAND( JUMP );
					labelPointer[data.size()] = it->second;
					if( debug ) printf( "\n Label used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined jump argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "jumptrue" )
			{
				READ_STRING_CONTINUE;
				auto it = labels.find( com );
				if( it != labels.end() )
				{
					PUSH_DATA_COMMAND( JUMPTRUE );
					labelPointer[data.size()] = it->second;
					if( debug ) printf( "\n Label used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined jump argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "jumpfalse" )
			{
				READ_STRING_CONTINUE;
				auto it = labels.find( com );
				if( it != labels.end() )
				{
					PUSH_DATA_COMMAND( JUMPFALSE );
					labelPointer[data.size()] = it->second;
					if( debug ) printf( "\n Label used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined jump argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "gotoxy" )
			{
				PUSH_DATA_COMMAND( GOTOXY );
			}
			else if( com == "clearscr" )
			{
				PUSH_DATA_COMMAND( CLEARSCR );
			}
			else if( com == "datacopy" )
			{
				PUSH_DATA_COMMAND( DATACOPY );
			}
			else if( com == "add" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( ADD );
			}
			else if( com == "sub" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( SUB );
			}
			else if( com == "div" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( DIV );
			}
			else if( com == "mul" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( MUL );
			}
			else if( com == "mod" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( MOD );
			}
			else if( com == "and" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( AND );
			}
			else if( com == "nand" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( NAND );
			}
			else if( com == "or" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( OR );
			}
			else if( com == "nor" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( NOR );
			}
			else if( com == "xor" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( XOR );
			}
			else if( com == "xnor" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( XNOR );
			}
			else if( com == "not" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( NOT );
			}
			else if( com == "pow" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( POW );
			}
			else if( com == "sqrt" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( SQRT );
			}
			else if( com == "shiftleft" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( SHIFTLEFT );
			}
			else if( com == "shiftright" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( SHIFTRIGHT );
			}
			else if( com == "equal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( EQUAL );
			}
			else if( com == "notequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( NOTEQUAL );
			}
			else if( com == "less" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( LESS );
			}
			else if( com == "grater" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( GRATER );
			}
			else if( com == "lessequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( LESSEQUAL );
			}
			else if( com == "graterequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( GRATEREQUAL );
			}
			else if( com == "stringequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( STRINGEQUAL );
			}
			else if( com == "stringnotequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( STRINGNOTEQUAL );
			}
			else if( com == "stringless" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( STRINGLESS );
			}
			else if( com == "stringgrater" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( STRINGGRATER );
			}
			else if( com == "stringlessequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( STRINGLESSEQUAL );
			}
			else if( com == "stringgraterequal" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( STRINGGRATEREQUAL );
			}
			else if( com == "tobool" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( TOBOOLEAN );
			}
			else if( com == "printchar" )
			{
				PUSH_DATA_COMMAND( PRINTCHAR );
			}
			else if( com == "printstring" )
			{
				PUSH_DATA_COMMAND( PRINTSTRING );
			}
			else if( com == "printint" )
			{
				PUSH_DATA_COMMAND( PRINTINT );
			}
			else if( com == "getchar" )
			{
				PUSH_DATA_COMMAND( GETCHAR );
			}
			else if( com == "getstring" )
			{
				PUSH_DATA_COMMAND( GETSTRING );
			}
			else if( com == "getint" )
			{
				PUSH_DATA_COMMAND( GETINT );
			}
			else if( com == " " || com == "code_end__" )
			{
				break;
			}
			else
			{
				break;
			}
		}
		
		delete[] fastData;
		
		for( auto f_it_ = variablePointer.begin(); f_it_ != variablePointer.end(); *f_it_++ )
		{
			for( auto it = f_it_->second.begin(); it != f_it_->second.end(); *it++ )
			{
				if( debug ) printf( "\n Variable used in function \"%s\" point: %lli  should be: %lli", f_it_->first.c_str(), (int64)(it->first), (int64)(variableId_variable[f_it_->first][it->second]) );
				SetIntAt( variableId_variable[f_it_->first][it->second], it->first );
			}
		}
		
		for( auto it = labelPointer.begin(); it != labelPointer.end(); *it++ )
		{
			if( debug ) printf( "\n Jump used in point: %lli  should be: %lli", (int64)(it->first), (int64)(labelId_label[it->second]) );
			SetIntAt( labelId_label[it->second], it->first );
		}
		
		for( auto it = functionPointer.begin(); it != functionPointer.end(); *it++ )
		{
			if( debug ) printf( "\n Function used in point: %lli  should be: %lli", (int64)(it->first), (int64)(functionId_function[it->second]) );
			SetIntAt( functionId_function[it->second], it->first );
		}
	}
	else
	{
		printf( "\n can not open file: \"%s\" ", fileName );
		printf( "\n [PRESS ENTER] " );
		getchar();
		return;
	}
	data.resize( data.size()+1 );
}



