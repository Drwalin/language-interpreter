
#prgagma once

void MyAssemblyLang::CompileComplexToPrimitive( const char * fileName, const char * fileNameAsm /*OUT*/ )
{
	std::ofstream out;
	std::ifstream code;
	code.open( fileName );
	out.open( fileNameAsm );
	if( code.good() && out.good() )
	{
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
		}
		
		uint64 usedNum = 0, i = 0;
		
		while( true )
		{
			usedNum = 0;
			if( cmd.size() >= 2 )
			{
				i = 0;
				if( cmd[1] == "=" || cmd[0] == "if" )
				{
					while( cmd[i] != ";" )
						++i;
					usedNum = i+1;
					
					if( cmd[1] == "=" )
					{
						i = 4;
						out << "\npush " << cmd[2];
					}
					else if( cmd[1] == "if" )
					{
						i = 3;
						out << "\npush " << cmd[1];
					}
					
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
							out << "\nnot";
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
					if( cmd[1] == "=" )
					{
						out << "\npop " << cmd[0];
					}
					else if( cmd[1] == "if" )
					{
						usedNum += 
						i = 3;
						out << "\npush " << cmd[1];
					}
					
					
				}
			}
			if( usedNum == 0 && cmd.size() >= 1 )
			{
				
			}
			cmd.erase( cmd.begin(), cmd.begin()+usedNum );
		}
	}
}



