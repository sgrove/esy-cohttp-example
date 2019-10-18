let addQueryParams = (queryParams, uri) =>
  List.fold_left(
    (run, next) => Uri.add_query_param(run, next),
    uri,
    queryParams,
  );

let addOptionalQueryParams = (queryParams, uri) =>
  List.fold_left(
    (run, next) =>
      switch (next) {
      | (_, [None]) => run
      | (key, [Some(param)]) => Uri.add_query_param(run, (key, [param]))
      | _ => failwith("Malformed optional argument in uriUtils")
      },
    uri,
    queryParams,
  );

let addHeader = (newHeader, headers) =>
  Cohttp.Header.add_list(headers, [newHeader]);

let addHeaders = (newHeaders, headers) =>
  Cohttp.Header.add_list(headers, newHeaders);

let addOptionalHeader = (newHeaderT, headers) =>
  switch (newHeaderT) {
  | None => headers
  | Some(newHeader) => Cohttp.Header.add_list(headers, [newHeader])
  };

let addOptionalHeaders = (newHeaderTs, headers) =>
  List.fold_left(
    (acc, next) =>
      switch (next) {
      | None => acc
      | Some(newHeader) => Cohttp.Header.add_list(acc, [newHeader])
      },
    headers,
    newHeaderTs,
  );
