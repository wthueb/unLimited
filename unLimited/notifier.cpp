#include "notifier.hpp"

// god i hate templates sometimes

void Callback::Run(HTTPRequestCompleted_t* pvParam)
{
    if (pvParam->m_nStatusCode == 200)
    {
        utils::printf("http response: 200 OK\n\n");
    }
    else
    {
        utils::printf("something went wrong with the http request...\n");
        utils::printf("error code: %d\n\n", pvParam->m_nStatusCode);
    }

    g_steam_http->ReleaseHTTPRequest(pvParam->m_hRequest);
}
