// Copyright 2014, Yahoo! Inc.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef YAHOO_CNET_CNET_RESPONSE_H_
#define YAHOO_CNET_CNET_RESPONSE_H_

#include "base/memory/ref_counted.h"
#include "net/http/http_response_info.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"
#include "yahoo/cnet/cnet_url_params.h"

struct CnetLoadTiming;

namespace net {
class GrowableIOBuffer;
class HttpResponseHeaders;
}

namespace cnet {

class Response : public base::RefCountedThreadSafe<Response> {
 public:
  Response(const std::string& initial_url,
      const GURL& original_url, const GURL& final_url,
      scoped_refptr<net::GrowableIOBuffer> read_buffer,
      const UrlParams& url_params,
      scoped_ptr<CnetLoadTiming> load_timing,
      const net::URLRequestStatus& status, int http_response_code,
      scoped_refptr<net::HttpResponseHeaders> response_headers,
      scoped_ptr<net::HttpResponseInfo> response_info);

  const std::string& initial_url() { return initial_url_; }
  const GURL& original_url() { return original_url_; }
  const GURL& final_url() { return final_url_; }
  const UrlParams& url_params() { return url_params_; }
  const CnetLoadTiming* load_timing() { return timing_.get(); }
  const net::URLRequestStatus& status() { return status_; }
  int http_response_code() { return http_response_code_; }
  scoped_refptr<net::HttpResponseHeaders> response_headers() {
    return response_headers_;
  }

  bool was_cached() const {
    return (response_info_ != NULL) && response_info_->was_cached;
  }

  bool was_fetched_via_proxy() const {
    return (response_info_ != NULL) && response_info_->was_fetched_via_proxy;
  }

  bool was_fetched_via_http() const {
    return (response_info_ != NULL) &&
        (response_info_->connection_info ==
            net::HttpResponseInfo::CONNECTION_INFO_HTTP1);
  }

  bool was_fetched_via_spdy() const {
    return (response_info_ != NULL) && response_info_->was_fetched_via_spdy;
  }

  bool was_fetched_via_quic() const {
    return (response_info_ != NULL) &&
        (response_info_->connection_info ==
            net::HttpResponseInfo::CONNECTION_INFO_QUIC1_SPDY3);
  }

  const char *response_body();
  int response_length();

 private:
  std::string initial_url_;
  GURL original_url_;
  GURL final_url_;
  scoped_refptr<net::GrowableIOBuffer> read_buffer_;
  UrlParams url_params_;
  scoped_ptr<CnetLoadTiming> timing_;
  net::URLRequestStatus status_;
  int http_response_code_;
  scoped_refptr<net::HttpResponseHeaders> response_headers_;
  scoped_ptr<net::HttpResponseInfo> response_info_;

  virtual ~Response();
  friend class base::RefCountedThreadSafe<Response>;
  DISALLOW_COPY_AND_ASSIGN(Response);
};


} // namespace cnet

#endif  // YAHOO_CNET_CNET_RESPONSE_H_
