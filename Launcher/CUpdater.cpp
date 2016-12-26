#include "../Shared/CCommon.h"
#include "../Shared/md5/md5.h"
#include "../Shared/CString.h"
#include "StdInc.h"

#include <curl\curl.h>

#include "CUpdater.h"

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
	size_t newLength = size*nmemb;
	size_t oldLength = s->size();
	try
	{
		s->resize(oldLength + newLength);
	}
	catch (std::bad_alloc &e)
	{
		return 0;
	}

	std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);
	return size*nmemb;
}

CUpdater::CUpdater(String path):
	m_modulePath(path)
{
}

CUpdater::~CUpdater()
{

}

/*
** This check if a new download is available
** Take path to the current DLL file as parameter
** Return @c true if download available, @c false if not.
*/
bool CUpdater::IsNewVersionAvailable()
{
	String currentHash = md5(m_modulePath, true).ToUpper();

	CURL *curl;
	CURLcode res;
	std::string s;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, MASTERLIST_HOST UPDATER_VERSION);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			return false;

		curl_easy_cleanup(curl);
		if (!currentHash.Compare(s.c_str()))
			return (false);
		
		return (true);
	}
	return (false);
}

/*
** This method return the distant link for new version of DLL
** Return the link
*/
String CUpdater::GetPathForNewVersion()
{
	CURL *curl;
	CURLcode res;
	std::string s;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, MASTERLIST_HOST UPDATER_FILE);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			return NULL;

		curl_easy_cleanup(curl);

		return String(s.c_str());
	}
}

/*
** This method proceed the auto updating of DLL file
** Return @c true if update succeed, @c false if not
*/
bool CUpdater::StartUpdating()
{
	String fileUrl = GetPathForNewVersion();

	CURL *curl;
	FILE *fp;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(m_modulePath.Get(), "wb");
		curl_easy_setopt(curl, CURLOPT_URL, fileUrl.Get());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		return (true);
	}
	return false;
}