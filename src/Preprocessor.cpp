
#pragma once

void MyAssemblyLang::Preprocessor( const char * fileName, const char * fileNameComplex /*out*/ )
{
	std::ofstream out;
	std::ifstream code;
	code.open( fileName );
	out.open( fileNameComplex );
	if( code.good() && out.good() )
	{
		if( debug ) printf( "\n Good\n" );
		
		std::vector < std::string > cmd;
		cmd.resize( 1000000 );
		cmd.resize( 0 );
		std::string com;
		int i;
		
		std::map < std::string, std::string > macro;
		
		while( !code.eof() )
		{
			com = "";
			code >> com;
			if( com == "" )
				break;
			cmd.push_back( com );
		}
		
		if( debug && false )
		{
			for( i = 0; i < cmd.size(); ++i )
			{
				printf( "\n cmd[%i] = \"%s\"", i, cmd[i].c_str() );
			}
		}
		
		if( debug ) printf( "\n Good\n" );
		
		while( cmd.size() > 0 )
		{
			if( cmd.front() == "#define" )
			{
				com = "";
				if( debug ) printf( "\n #define ..." );
				for( i = 2; i < cmd.size(); ++i )
				{
					if( cmd[i] == "#enddef" )
					{
						com.resize( com.size() - 1 );
						break;
					}
					else
					{
						com += cmd[i];
						com += " ";
					}
				}
				
				macro[cmd[1]] = com;
				
				if( debug ) printf( "\b\b\b \"%s\" -> \"%s\"", cmd[1].c_str(), com.c_str() );
				
				cmd.erase( cmd.begin(), cmd.begin() + i + 1);
			}
			else if( cmd.front() == "#include" )
			{
				com = "";
				for( i = 1; i <= cmd.size(); ++i )
				{
					if( cmd[i] == "#endinclude" )
					{
						com.resize( com.size() - 1 );
						break;
					}
					else
					{
						com += cmd[i];
						com += " ";
					}
				}
				
				cmd.erase( cmd.begin(), cmd.begin() + i + 1 );
				
				std::ifstream include;
				include.open( com.c_str() );
				
				if( debug ) printf( "\n #include file \"%s\"", com.c_str() );
				
				if( include.good() )
				{
					std::vector < std::string > tempInclude;
					for( i = 0;; ++i )
					{
						com = "";
						include >> com;
						if( com == "" )
							break;
						tempInclude.push_back( com );
					}
					include.close();
					
					for( i = 0; i < tempInclude.size(); ++i )
					{
						cmd.insert( cmd.begin() + i, tempInclude[i] );
					}
					
					tempInclude.clear();
				}
				else
				{
					printf( "\n Can not include file \"%s\" ", com.c_str() );
				}
				
			}
			else if( cmd.front() == "#undef" )
			{
				macro.erase( cmd[1] );
				cmd.erase( cmd.begin(), cmd.begin() + 2 );
			}
			else
			{
				auto it = macro.find( cmd.front() );
				if( it != macro.end() )
				{
					out << " " << it->second << " ";
				}
				else
				{
					out << cmd.front() << " ";
				}
				cmd.erase( cmd.begin(), cmd.begin() + 1 );
			}
			
		}
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



