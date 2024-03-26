#include "carpc/tools/parameters/Params.hpp"
#include "ConnectionProcessor.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



using tStart = void (*)( int, char**, char** );
using tExit = void (*)( void );

void preinit( int argc, char** argv, char** envp )
{
   auto params = carpc::tools::parameters::Params( argc, argv, envp );

   const carpc::trace::eLogStrategy trace_strategy = carpc::trace::log_strategy_from_string(
         params.value_or( "trace_log", "CONSOLE" )
      );
   const std::size_t trace_buffer = static_cast< std::size_t >(
         std::stoll( params.value_or( "trace_buffer", "4096" ) )
      );
   const std::string trace_app_name = params.value_or( "trace_app_name", "SBR" );
   const carpc::trace::eLogLevel trace_level = carpc::trace::log_level_from_number(
         std::stoll( params.value_or( "trace_level", "6" ) )
      );
   carpc::trace::Logger::init( trace_strategy, trace_app_name, trace_buffer, trace_level );

   MSG_INF( "preinit_array" );
}
__attribute__(( section(".preinit_array") )) tStart __preinit__ = preinit;

void init( int argc, char** argv, char** envp )
{
   MSG_INF( "init_array" );
}
__attribute__(( section(".init_array") )) tStart __init__ = init;

void fini( )
{
   MSG_INF( "fini_array" );
}
__attribute__(( section(".fini_array") )) tExit __fini__ = fini;

void __constructor__( ) __attribute__(( constructor(101) ));
void __constructor__( )
{
   MSG_INF( "constructor" );
}

void __destructor__( ) __attribute__(( destructor(101) ));
void __destructor__( )
{
   MSG_INF( "destructor" );
}



int main( int argc, char** argv, char** envp )
{
   carpc::tools::parameters::Params params( argc, argv, envp );
   params.print( );

   carpc::os::os_linux::socket::configuration socket_configuration{
      carpc::os::os_linux::socket::socket_domain_from_string(
            params.value( "ipc_servicebrocker_domain" )
         ),
      carpc::os::os_linux::socket::socket_type_from_string(
            params.value( "ipc_servicebrocker_type" )
         ),
      static_cast< int >( std::stoll( params.value( "ipc_servicebrocker_protocole" ) ) ),
      params.value( "ipc_servicebrocker_address" ),
      static_cast< int >( std::stoll( params.value( "ipc_servicebrocker_port" ) ) )
   };

   std::size_t buffer_size = static_cast< std::size_t >( std::stoll(
         params.value( "ipc_servicebrocker_buffer_size" )
      ) );

   ConnectionProcessor conn( socket_configuration, buffer_size );
   conn.init( );
   conn.connection_loop( );



   return 0;
}
