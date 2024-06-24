#pragma once

#include <cstdint>
#include <thread>

/**
 * \ingroup pbc
 * \defgroup pbc_sock Socket
 * \brief TCP socket handling
 * \{
 */

/**
 * \brief Asynchronous puzzle box socket connection
 * \note Once connected, this class will call \c i2c_recv() when a complete I2C
 * message has been received
 */
class PBSocket {
public:
	PBSocket();
	PBSocket(const char * addr, uint16_t port);
	virtual ~PBSocket();

	//! Configure target server
	void set_server(const char * addr, uint16_t port);

	//! Attempt to connect to server and start \c sock_task() in a thread
	void sock_connect();

	/**
	 * \brief Send data over the TCP connection
	 *
	 * \param buf Data to send
	 * \param buf_sz Size of \p buf in bytes
	 */
	void send(const char * buf, size_t buf_sz);

private:
	/**
	 * \brief Continously read from the TCP socket and read \ref i2ctcp messages
	 * using \c i2ctcp_read().
	 *
	 * Once a complete message has been parsed, \c i2c_recv() is called with the
	 * complete message. This message is automatically free'd after \c i2c_recv()
	 * returns.
	 *
	 * \note This function is run in a separate thread
	 */
	void sock_task();
	//! Close the socket
	void sock_close();

	//! Pointer to thread running \c sock_task()
	std::thread* _thread = nullptr;

	/**
	 * \brief IP address of server to connect to
	 *
	 * \note This member must contain an IP address, as no hostname resolution is
	 * done in pbc.
	 */
	const char * _addr = NULL;
	//! Port number of server to connect to
	uint16_t _port = 0;

	//! Unix file descriptor of opened socket
	int _fd = -1;
};

//! Singleton \c PBSocket instance
extern PBSocket* sock;

/// \}

