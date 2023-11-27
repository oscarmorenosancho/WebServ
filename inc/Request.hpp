/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoreno- <omoreno-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:16:44 by omoreno-          #+#    #+#             */
/*   Updated: 2023/11/27 13:41:57 by omoreno-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REQUEST_HPP_
# define _REQUEST_HPP_
# include <string>
# include <Client.hpp>
# include "Headers.hpp"

class Request
{
public:
	typedef enum e_status
	{
		IDLE,
		FD_BOND,
		RECVD_START,
		RECVD_REQ_LINE,
		RECVD_HEADER,
		RECVD_CHUNK_SIZE,
		RECVD_CHUNK,
		RECVD_LAST_CHUNK,
		RECVD_ALL,
		RESP_RENDERED
	}	t_status;
private:
	t_status					status;
	Client						*client;
	size_t						pending;
	size_t 						chunkSize;
	std::string					received;
	std::string					protocol;
	std::string					method;
	std::string					route;
	std::string					query;
	Headers						headers;
	std::string					body;
	bool						badRequest;
	void 								parseRoute(void);
	void 								parseFirstLine(const std::string &line);
	void 								parseHeader(const std::string &line);
	bool								getLine(std::string& line);
	bool								processLineOnFdBond(const std::string &line);
	bool								processLineOnRecvdStart(const std::string &line);
	bool								processLineOnRecvdReqLine(const std::string &line);
	bool								processLineOnRecvdHeader(const std::string &line);
	bool								processLineOnRecvdChunkSize(const std::string &line);
	bool								processLineOnRecvdChunk(const std::string &line);
	bool								processLineOnRecvdLastChunk(const std::string &line);
	bool								processLine(const std::string& line);
public:
	Request(void);
	Request(Client *client);
	~Request();
	Request(const Request& b);
	Request&	operator=(const Request& b);
	int bindClient(Client* cli);
	bool appendRecv(const std::string &recv);
	t_status							getStatus() const;
	Client*								getClient() const;
	std::string							getProtocol() const;
	std::string							getMethod() const;
	std::string							getRoute() const;
	std::string							getQuery() const;
	const Headers&						getHeaders() const;
	size_t								getBodyLength() const;
	std::string							getBody() const;
	bool								isCompleteRecv() const;
	bool								isReadyToSend() const;
	bool								isReceiving() const;
	std::string							toString();
	void								setBody(const std::string& content);
	void								setReadyToSend();
	int									setDummyRecv();
	void								logStatus();
};

#endif
