
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
	std::map < std::string, uint64 > labels;				// name - id
	std::map < uint64, uint64 > labelId_label;				// id - machine code pointer
	std::map < uint64, uint64 > labelPointer;				// in code pointer to place were it's used - id
	
	std::map < std::string, uint64 > variables;				// name - pointer
	std::map < uint64, uint64 > variableId_variable;		// id - machine code pointer
	std::map < uint64, uint64 > variablePointer;			// in code pointer to place were it's used - id
	
	std::ifstream code;
	code.open( fileName );
	if( code.good() )
	{
		uint64 labelsCounter = 0, variablesCounter = 0;
		std::string com;
		while( true )
		{
			READ_STRING_BREAK;
			
			if( com == "label" )
			{
				READ_STRING_BREAK;
				labels[com] = labelsCounter;
				++labelsCounter;
			}
			else if( com == "var" )
			{
				READ_STRING_BREAK;	// var type
				READ_STRING_BREAK;	// var name
				variables[com] = variablesCounter;
				++variablesCounter;
			}/*
			else if( com == "func" )
			{
				
			}*/
		}
		
		code.seekg( 0, code.beg );
		
		while( true )
		{
			com = "";
			READ_STRING_BREAK;
			
			if( com == "label" )
			{
				READ_STRING_CONTINUE;
				auto it = labels.find(com);
				if( it != labels.end() )
				{
					labelId_label[it->second] = data.size();
					printf( "\n Label: \"%s\"  pointer: %lli ", com.c_str(), (int64)data.size() );
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
				auto it = variables.find( com );
				if( it != variables.end() )
				{
					variableId_variable[it->second] = data.size();
					printf( "\n Variable: \"%s\"  pointer: %lli ", com.c_str(), (int64)data.size() );
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
					printf( "\n Undefined variable name in variable declaration \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
			}/*
			else if( com == "func" )
			{
				
			}*/
			else if( com == "end" )		//////////////////////
			{
				PUSH_DATA_COMMAND( END );
			}
			else if( com == "push" )
			{
				READ_STRING_CONTINUE;
				auto it = variables.find( (com[0]=='&'||com[0]=='*') ? com.c_str()+1 : com );
				if( it != variables.end() )
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
					variablePointer[data.size()] = it->second;
					printf( "\n variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
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
				/*
				READ_STRING_CONTINUE;
				auto it = variables.find( com );
				if( it != variables.end() )
				{
					PUSH_DATA_COMMAND( POPGLOBAL );
					variablePointer[data.size()] = it->second;
					printf( "\n variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined pop argument \"%s\"  at byte: %i", com.c_str(), (int)code.tellg() );
					getchar();
					return;
				}
				*/
				
				READ_STRING_CONTINUE;
				auto it = variables.find( (com[0]=='*') ? com.c_str()+1 : com );
				if( it != variables.end() )
				{
					if( com[0] == '*' )
					{
						PUSH_DATA_COMMAND( POPGLOBALADRESS );
					}
					else
					{
						PUSH_DATA_COMMAND( POPGLOBAL );
					}
					variablePointer[data.size()] = it->second;
					printf( "\n variable used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
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
			}/*
			else if( com == "fileopen" )
			{
				
			}
			else if( com == "fileclose" )
			{
				
			}
			else if( com == "loadfromfile" )
			{
				
			}
			else if( com == "sevetofile" )
			{
				
			}
			else if( com == "getfilesize" )
			{
				
			}
			else if( com == "jumpfile" )
			{
				
			}*/
			else if( com == "allocmem" )
			{
				PUSH_DATA_COMMAND( ALLOCATEMEMORY );
			}
			else if( com == "freemem" )
			{
				PUSH_DATA_COMMAND( FREEMEMORY );
			}
			else if( com == "jump" )
			{
				READ_STRING_CONTINUE;
				auto it = labels.find( com );
				if( it != labels.end() )
				{
					PUSH_DATA_COMMAND( JUMP );
					labelPointer[data.size()] = it->second;
					printf( "\n Label used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
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
					printf( "\n Label used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
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
					printf( "\n Label used in place: %lli  with id: %lli", (int64)data.size(), (int64)(it->second) );
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
			}/*
			else if( com == "move" )
			{
				
			}*/
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
			}/*
			else if( com == "comparestring" )
			{
				
			}*/
			else if( com == "tobool" )
			{
				PUSH_DATA_COMMAND( ALU );
				PUSH_DATA_COMMAND( TOBOOLEAN );
			}
			else if( com == "printintnewline" )
			{
				PUSH_DATA_COMMAND( PRINTINTNEWLINE );
			}
			else if( com == "scanintkeyboard" )
			{
				PUSH_DATA_COMMAND( SCANINTKEYBOARD );
			}
			else if( com == "" )
			{
				break;
			}
		}
		
		for( auto it = variablePointer.begin(); it != variablePointer.end(); *it++ )
		{
			printf( "\n Variable used in point: %lli  should be: %lli", (int64)(it->first), (int64)(variableId_variable[it->second]) );
			SetIntAt( variableId_variable[it->second], it->first );
		}
		
		for( auto it = labelPointer.begin(); it != labelPointer.end(); *it++ )
		{
			printf( "\n Jump used in point: %lli  should be: %lli", (int64)(it->first), (int64)(labelId_label[it->second]) );
			SetIntAt( labelId_label[it->second], it->first );
		}
	}
	else
	{
		printf( "\n can not open file: \"%s\" ", fileName );
		printf( "\n [PRESS ENTER] " );
		getchar();
		return;
	}
}



