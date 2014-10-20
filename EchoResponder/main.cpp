#include "../EventEngines/SyncEventHandler.hpp"
#include "../EventEngines/AsyncEventHandler.hpp"
#include "../IO/TcpSocket.hpp"

class test: public AsyncEventHandler<TcpSocket, std::string>
{
	public:
		test(TcpSocket::Ptr p): AsyncEventHandler(p){}
		void handle(const std::string& p)
		{
		}
};

int main(int, char**)
{
	TcpSocket::Ptr p(new TcpSocket());
	test t(p);
	return 0;
}

