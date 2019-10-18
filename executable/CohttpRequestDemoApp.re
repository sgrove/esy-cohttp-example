open Async_kernel;
open Cohttp_async;

let defaultQuery = {|
query NpmPackageInfo($name: String!) {
  npm {
    package(name: $name) {
      name
      downloads {
        lastMonth {
          count
          end
        }
      }
      repository {
        sourceRepository {
          ... on GitHubRepository {
            description
            issues(first: 10, states: [OPEN]) {
              nodes {
                title
              }
            }
          }
        }
      }
    }
  }
}

mutation WinWinWin {
  spotify {
    playTrack(
      input: {
        # win win win
        trackIds: ["0RIHDrcRAXtUlnkvTYPW1a"]
        positionMs: 69500
      }
    ) {
      player {
        isPlaying
      }
    }
  }
}
|};

let defaultVariables = {|{"name": "graphql"}|};

let makeRequest = (query, variablesString, operationName, ()) => {
  print_endline(
    Format.sprintf(
      "Making GraphQL request over HTTP with query:\n%s\n",
      query,
    ),
  );

  let variables =
    variablesString
    |> Base.Option.value(~default=defaultVariables)
    |> Yojson.Basic.from_string;

  let operationName =
    Base.Option.value(~default="NpmPackageInfo", operationName);

  /* let operationName = "NpmPackageInfo"; */

  let%map message =
    switch%map (
      OneGraphClient.req(
        ~appId="0b33e830-7cde-4b90-ad7e-2a39c57c0e11",
        ~query,
        ~variables,
        ~operationName,
        (),
      )
    ) {
    | Error(err) => Format.sprintf("Hit an HTTP error speaking to OneGraph")
    | Ok(json) =>
      let graphQLResponseData = Yojson.Basic.Util.(json |> member("data"));
      let graphQLResponseErrors =
        Yojson.Basic.Util.(json |> member("errors"));

      Format.sprintf(
        {|GraphQL Response:
-------
Data: %s

Errors: %s
|},
        Yojson.Basic.pretty_to_string(graphQLResponseData),
        Yojson.Basic.pretty_to_string(graphQLResponseErrors),
      );
    };

  print_endline(message);
};

let () = {
  module Command = Async_extra.Command;
  Command.async_spec(
    ~summary="Make a GraphQL request to OneGraph over HTTP(s)",
    Command.Spec.(
      empty
      +> flag(
           "-q",
           optional_with_default(defaultQuery, string),
           ~doc="string GraphQL query to submit",
         )
      +> flag(
           "-v",
           optional(string),
           ~doc="string JSON variables to submit",
         )
      +> flag(
           "-n",
           optional(string),
           ~doc="string OperationName in the GraphQL query",
         )
    ),
    makeRequest,
  )
  |> Command.run;
};
