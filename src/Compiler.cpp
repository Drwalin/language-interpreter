
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

#define PUSH_UINT64_TO_DATA(a__)		{\
											uint64 a__f_macro = data.size(), b__f_macro = 0;\
											data.resize( a__f_macro+8 );\
											for( b__f_macro = 0; b__f_macro < 8; ++b__f_macro )\
												data[a__f_macro+b__f_macro] = (a__>>(b__f_macro<<3))&255;\
										}

#define PUSH_DATA_COMMAND(a__)		data.resize( data.size() + 1 );\
									data.back( a__ );

void MyAssemblyLang::PrimitiveCompiler( const char * fileName )
{
	std::map < std::string, uint64 > labels;				// name - id
	std::map < uint64, uint64 > labelId_label;				// id - machine code pointer
	std::map < uint64, uint64 > labelPointer;				// in code pointer to place were it's used - id
	
	std::map < std::string, uint64 > variables;				// name - pointer
	std::map < uint64, uint64 > variableId_label;			// id - machine code pointer
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
		
		code.seekg( 0, code.beg() );
		
		while( true )
		{
			READ_STRING_RETURN;
			
			if( com == "label" )
			{
				READ_STRING_RETURN;
				auto it = labels.find(com);
				if( it != labels.end() )
				{
					labelId_label[it->second] = data.size();
				}
				else
				{
					printf( "\n Undefined label name \"%s\"  at byte: %i", com.c_str(), code.tellg() );
					getchar();
					return;
				}
			}
			else if( com == "var" )		/////////////////////////////
			{
				READ_STRING_RETURN;
				std::string varType = com;
				READ_STRING_RETURN;
				auto it = variables.find( com );
				if( it != variables.end() )
				{
					variableId_label[com] = data.size();
					if( varType == "string" )
					{
						READ_STRING_RETURN;
						if( false )
						{
							VAR_STRING_FORMAT_ERROR:
								printf( "\n Undefined string format \"%s\"  at byte: %i", com.c_str(), code.tellg() );
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
								data.resize( dst.size()+1 );
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
						long long int temp = 0;
						code >> temp;
						PUSH_UINT64_TO_DATA(temp);
					}
					else if( varType == "bytes" )
					{
						byte temp;
						uint64 temp1 = 0, temp2 = 0, temp3;
						code >> temp1;
						code >> temp2;
						temp = temp1;
						temp1 = data.size();
						data.resize( temp1 + temp2 );
						for( temp3 = 0; temp3 < temp2; ++ temp3 )
							data[temp1+temp3] = temp;
					}
					else
					{
						printf( "\n Undefined variable type \"%s\"  at byte: %i", varType.c_str(), code.tellg() );
						getchar();
						return;
					}
				}
				else
				{
					printf( "\n Undefined variable name in variable declaration \"%s\"  at byte: %i", com.c_str(), code.tellg() );
					getchar();
					return;
				}
			}/*
			else if( com == "func" )
			{
				
			}*/
			else if( com == "end" )		//////////////////////
			{
				data.push_back( END );
			}
			else if( com == "push" )
			{
				READ_STRING_RETURN;
				auto it = variables.find( (com[0]=='*') ? com.c_str()+1 : com );
				if( it != variables.end() )
				{
					if( com[0] == '&' )
					{
						PUSH_DATA_COMMAND( PUSHADRESSGLOBAL );
					}
					else
					{
						PUSH_DATA_COMMAND( PUSHGLOBAL );
					}
					variablePointer[data.size()] = it->second
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
						printf( "\n Undefined push argument \"%s\"  at byte: %i", com.c_str(), code.tellg() );
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
				READ_STRING_RETURN;
				auto it = variables.find( com );
				if( it != variables.end() )
				{
					variablePointer[data.size()] = it->second
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined pop argument \"%s\"  at byte: %i", com.c_str(), code.tellg() );
					getchar();
					return;
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
			else if( com == "jump" )
			{
				READ_STRING_RETURN;
				auto it = labels.find( com );
				if( it != labels.end() )
				{
					labelPointer[data.size()] = it->second
					uint64 temp1 = data.size(), temp2;
					data.resize( temp1 + 8 );
					for( temp2 = 0; temp2 < 8; ++temp2 )
					{
						data[temp1+temp2] = 0;
					}
				}
				else
				{
					printf( "\n Undefined jump argument \"%s\"  at byte: %i", com.c_str(), code.tellg() );
					getchar();
					return;
				}
			}/*
			else if( com == "jumptrue" )
			{
				
			}
			else if( com == "jumpfalse" )
			{
				
			}
			else if( com == "move" )
			{
				
			}*/
			else if( com == "add" )
			{
				
			}
			else if( com == "sub" )
			{
				
			}
			else if( com == "div" )
			{
				
			}
			else if( com == "mod" )
			{
				
			}
			else if( com == "and" )
			{
				
			}
			else if( com == "nand" )
			{
				
			}
			else if( com == "or" )
			{
				
			}
			else if( com == "nor" )
			{
				
			}
			else if( com == "xor" )
			{
				
			}
			else if( com == "xnor" )
			{
				
			}
			else if( com == "not" )
			{
				
			}
			else if( com == "pow" )
			{
				
			}
			else if( com == "sqrt" )
			{
				
			}
			else if( com == "log" )
			{
				
			}
			else if( com == "shiftleft" )
			{
				
			}
			else if( com == "shiftright" )
			{
				
			}
			else if( com == "equal" )
			{
				
			}
			else if( com == "notequal" )
			{
				
			}
			else if( com == "less" )
			{
				
			}
			else if( com == "grater" )
			{
				
			}
			else if( com == "lessequal" )
			{
				
			}
			else if( com == "graterequal" )
			{
				
			}
			else if( com == "comparestring" )
			{
				
			}
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



