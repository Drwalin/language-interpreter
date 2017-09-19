
#pragma once

void MyAssemblyLang::CompileComplexToPrimitive( const char * fileName, const char * fileNameAsm /*OUT*/ )
{
	std::ofstream out;
	std::ifstream code;
	code.open( fileName );
	out.open( fileNameAsm );
	if( code.good() && out.good() )
	{
		printf( "\n Good\n" );
		
		std::vector < std::string > cmd;
		cmd.resize( 100000 );
		cmd.resize( 0 );
		std::string com;
		
		while( !code.eof() )
		{
			com = "";
			code >> com;
			if( com == "" )
				break;
			cmd.push_back( com );
			//printf( "\n %s", com.c_str() );
		}
		
		printf( "\n Good\n" );
		
		uint64 usedNum = 0, i = 0;
		
		while( cmd.size() > 0 )
		{
			usedNum = 0;
			if( cmd.size() > 2 )
			{
				i = 0;
				if( cmd[1] == "=" || cmd[0] == "if" )
				{
					while( cmd[i] != ";" && cmd[i] != "endif" )
						++i;
					
					if( cmd[1] == "=" )
					{
						usedNum = i;
						i = 4;
						out << "\npush " << cmd[2];
					}
					else if( cmd[0] == "if" )
					{
						usedNum = i-1;
						i = 3;
						out << "\npush " << cmd[1];
					}
					
					for( i -= 1; i < usedNum; ++i )
					{
						com = cmd[i];
						std::cout << "\n " << com;
						if( com == "+" )
							com = "\nadd";
						else if( com == "-" )
							com = "\nsub";
						else if( com == "*" )
							com = "\nmul";
						else if( com == "/" )
							com = "\ndiv";
						else if( com == "%" )
							com = "\nmod";
						else if( com == "<<" )
							com = "\nshiftleft";
						else if( com == ">>" )
							com = "\nshiftright";
						else if( com == "&" )
							com = "\nand";
						else if( com == "!&" )
							com = "\nnand";
						else if( com == "|" )
							com = "\nor";
						else if( com == "!|" )
							com = "\nnor";
						else if( com == "^" )
							com = "\nxor";
						else if( com == "!^" )
							com = "\nxnor";
						else if( com == "pow" )
							com = "\npow";
						else if( com == "==" )
							com = "\nequal";
						else if( com == "!=" )
							com = "\nnotequal";
						else if( com == "<" )
							com = "\nless";
						else if( com == "<=" )
							com = "\nlessequal";
						else if( com == ">" )
							com = "\ngrater";
						else if( com == ">=" )
							com = "\ngraterequal";
						else if( com == "!" )		//////
						{
							out << "\nnot";
							continue;
						}
						else if( com == "sqrt" )	//////
						{
							out << "\nsqrt";
							continue;
						}
						else
						{
							out << "\npush " << cmd[i];
							continue;
						}
						
						out << "\npush " << cmd[i+1];
						out << com;
						++i;
						continue;
					}
					
					/*
					for( ; i < usedNum; i+=2 )
					{
						out << "\npush " << cmd[i];
						com = cmd[i-1];
						if( com == "+" )
							out << "\nadd";
						else if( com == "-" )
							out << "\nsub";
						else if( com == "*" )
							out << "\nmul";
						else if( com == "/" )
							out << "\ndiv";
						else if( com == "%" )
							out << "\nmod";
						else if( com == "<<" )
							out << "\nshiftleft";
						else if( com == ">>" )
							out << "\nshiftright";
						else if( com == "&" )
							out << "\nand";
						else if( com == "!&" )
							out << "\nnand";
						else if( com == "|" )
							out << "\nor";
						else if( com == "!|" )
							out << "\nnor";
						else if( com == "^" )
							out << "\nxor";
						else if( com == "!^" )
							out << "\nxnor";
						else if( com == "!" )
						{
							out << "\nnot";
						}
						else if( com == "pow" )
							out << "\npow";
						else if( com == "==" )
							out << "\nequal";
						else if( com == "!=" )
							out << "\nnotequal";
						else if( com == "<" )
							out << "\nless";
						else if( com == "<=" )
							out << "\nlessequal";
						else if( com == ">" )
							out << "\ngrater";
						else if( com == ">=" )
							out << "\ngraterequal";
					}
					*/
					
					if( cmd[1] == "=" )
					{
						out << "\npop " << cmd[0] << "\n";
					}
					else if( cmd[0] == "if" )
					{
						out << "\njumptrue " << cmd[usedNum] << "\n";
						usedNum += 2;
					}
				}
				else
				{
					usedNum = 1;
					out << cmd[0] << " ";
				}
			}
			else
			{
				usedNum = 1;
				out << cmd[0] << " ";
			}
			std::cout << "\n UseNum: " << usedNum << " :: " << cmd.size();
			cmd.erase( cmd.begin(), cmd.begin()+usedNum );
		}
		
		out << "\nCODE_END__";
	}
	else if( !code.good() && !out.good() )
	{
		printf( "\n BOTH not\n" );
	}
	else if( !code.good() )
	{
		printf( "\n CODE not\n" );
	}
	else if( !out.good() )
	{
		printf( "\n OUT not\n" );
	}
}



