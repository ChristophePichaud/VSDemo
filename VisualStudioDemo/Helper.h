#pragma once
class CHelper
{
public:
	CHelper();
	virtual ~CHelper();

public:
	static std::vector<std::string> Split(std::string str, std::string token);
};

