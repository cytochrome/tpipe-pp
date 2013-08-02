#include <iostream>
#include <cstdio>

#include <boost/program_options.hpp>

static const char* kVERSION_STRING =
    "tpipe++ version 0.0.1"
;

namespace P = boost::program_options;

typedef std::vector<std::string> StringV;
typedef std::vector<FILE*>     FileV;

void show_version();
void disable_buffer(FILE* fp,bool doBuffer);
int  tpipe_main( StringV& pipeCmds, bool force, bool flush);

int main(int argc, char** argv){

    P::variables_map param;
    P::options_description opt_v("Options");

    try{
        /* parse arguments */
        opt_v.add_options()
                ("version,v",  "show version.")
                ("help,h",     "show this message.")
                ("buffer,b",   "stop disabling buffering.(doesnt means do buffering)")
                ("force,f",    "ignore error.")
            ;

        P::options_description opt_h("hidden-options");
        opt_h.add_options()
                ("command-line", P::value< StringV >()->default_value(StringV(),"command-line"), "command line")
            ;

        P::options_description opt("options");
        opt.add(opt_v).add(opt_h);

        P::positional_options_description opt_p;
        opt_p.add("command-line", -1);

        P::parsed_options opt_parsed = P::command_line_parser(argc,argv)
            .options(opt)
            .positional(opt_p)
            .run();

        P::store(opt_parsed,param);
        P::notify(param);
    }
    catch(std::exception& e) {
        std::cerr << "fatal:" << e.what() << std::endl;
        return (-1);
    }    

    StringV pipeCmds  = param["command-line"].as< StringV >();
    bool    flush = param.count("buffer") == 0;
    bool    force = param.count("force")  != 0;

    // version
    if ( param.count("version") ){
        show_version();
        return 0;
    }

    // help
    if (param.count("help"))  goto usage;

    // no commands
    if ( pipeCmds.size() < 1 ){
        std::cerr << "at least 1 command-line/s are needed." << std::endl;
        goto usage;
    }

    // do main
    return tpipe_main(pipeCmds,force,flush);

usage:
    std::cerr << "Usage: " 
            << argv[0]
            << " [options] command1 [command2] ..."
            << std::endl
            << std::endl
            << opt_v << std::endl;
    return 0;

}


void show_version(){
    std::cout
        << kVERSION_STRING
        << std::endl;
}


// disable buffering
void disable_buffer(FILE* fp,bool doDisable){
    if( doDisable ){
        setvbuf(fp,NULL,_IONBF,0);
    }
}

struct PipeCreator {
    FileV& pipes_;
    bool force_;
    bool flush_;
    PipeCreator( FileV& pipes, bool force, bool flush) :
        pipes_(pipes),
        force_(force),
        flush_(flush)
    {}

    bool operator() ( std::string& s ){
        std::FILE* pipe = ::popen(s.c_str(),"w");

        if( pipe == (FILE*)NULL ){
            std::cerr << "error: cannot open pipe for '" << s << "'" << std::endl;
            return !force_ && true;
        }

        disable_buffer( pipe, flush_ );
        pipes_.push_back(pipe);

        return false;
    }
};

struct PipePrinter {
    int& c_;
    PipePrinter( int& c ): c_(c) {}
    void operator() (FILE* fp){
        std::fputc(c_,fp);
    }
};

// main procudure
int tpipe_main( StringV& pipeCmds, bool force, bool flush){

    FileV pipes;

    // create pipes
    {
        StringV::iterator i = 
            std::find_if(pipeCmds.begin(),pipeCmds.end(),PipeCreator(pipes,force,flush));

        // failed
        if( !force && i != pipeCmds.end()){ 
            goto stop_by_error;
        }
    }

    // set buffering
    {
        disable_buffer( stdin,  flush );
        disable_buffer( stdout, flush );
    }

    // main
    {
        int ch = std::fgetc(stdin);
        PipePrinter pp(ch);

        while( ch != EOF ){
            std::fputc(ch,stdout);
            std::for_each(pipes.begin(),pipes.end(),pp);
            ch = std::fgetc(stdin);
        }
    }

    // end
    fclose(stdout);
    std::for_each(pipes.begin(),pipes.end(),::pclose);
	return 0;

stop_by_error:
    std::cerr << "an error occured. stop." <<  std::endl;
    fclose(stdout);
    std::for_each(pipes.begin(),pipes.end(),::pclose);
    return (-1);
}


