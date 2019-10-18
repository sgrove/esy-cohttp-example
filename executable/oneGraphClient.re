open Async;
module Option = Base.Option;
module Json = Yojson.Basic;
module Client = Cohttp_async.Client;

"show_metrics=true&show_api_requests=false";

let endpointUri = Uri.of_string("https://serve.onegraph.com/graphql");

let invalidJSONError =
  Error(Json.from_string({|[{"message":"Invalid JSON from OneGraph"}]|}));

let req =
    (
      ~appId: string,
      ~authToken=?,
      ~variables=?,
      ~operationName=?,
      ~query: string,
      (),
    ) => {
  let bodyString =
    Yojson.Basic.to_string(
      `Assoc([
        ("query", `String(query)),
        ("variables", Option.value(~default=`Null, variables)),
        (
          "operationName",
          operationName
          |> Option.map(~f=operationName => `String(operationName))
          |> Option.value(~default=`Null),
        ),
      ]),
    );
  let body = bodyString |> Cohttp_async.Body.of_string;
  let headers =
    Cohttp.Header.of_list([
      ("User-Agent", "reason-native-http-client/0.1"),
      ("Content-Type", "application/json"),
    ])
    |> UriUtils.addOptionalHeader(
         Option.map(authToken, authToken =>
           ("Authentication", Format.sprintf("Bearer %s", authToken))
         ),
       );

  let uri = endpointUri |> UriUtils.addQueryParams([("app_id", [appId])]);

  print_endline(
    Format.asprintf(
      "Sending Query:Uri: %a\nHeaders: %a\nBody: %s",
      Uri.pp_hum,
      uri,
      Cohttp.Header.pp_hum,
      headers,
      bodyString,
    ),
  );

  Client.post(uri, ~headers, ~body)
  >>= (
    ((resp, body)) =>
      Cohttp_async.Body.to_string(body)
      >>| (
        body =>
          try({
            let code =
              resp |> Cohttp.Response.status |> Cohttp.Code.code_of_status;

            print_endline(
              Format.asprintf(
                "OneGraph response (%d status):\nUri: %a\nHeaders: %a\n",
                code,
                Uri.pp_hum,
                uri,
                Cohttp.Header.pp_hum,
                headers,
              ),
            );

            let json = Json.from_string(body);

            Ok(json);
          }) {
          | _ex => invalidJSONError
          }
      )
  );
};

let query = (~query) => req(~query);

let mutation = (~mutation) => req(~query=mutation);
