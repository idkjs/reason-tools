open Core;

open LocalDom;

let mapOrElse = (resolve, reject, item) =>
  switch (item) {
  | Some(res) => resolve(res)
  | None => reject()
  };

let getSelection = () =>
  Promise.make((resolve, reject) =>
    Chrome.Tabs.executeScript(
      {"code": "window.getSelection().toString()"}, maybeMaybeArray => {
      maybeMaybeArray
      |> Js.Nullable.toOption
      |> Js.Option.andThen((. maybeArray) =>
           Js.Array.findi((_, index) => index === 0, maybeArray)
         )
      |> Js.Option.andThen((. s) => s == "" ? None : Some(s))
      |> mapOrElse(resolve, reject)
    })
  );

let getLatestInput = () =>
  Promise.make((resolve, reject) =>
    Protocol.Storage.queryLatestInput(maybeInput =>
      maybeInput |> mapOrElse(resolve, reject)
    )
  );

let refmt =
    (
      input,
      ~inLang=RefmtShared.UnknownLang,
      ~inType=RefmtShared.UnknownType,
      ~outLang=RefmtShared.UnknownLang,
      cb,
    ) => {
  Protocol.Refmt.send(input, ~inLang, ~inType, ~outLang, res =>
    switch (res) {
    | Belt.Result.Ok({outText, inLang, outLang}) =>
      cb(outText, inLang, outLang)
    | Belt.Result.Error(error) =>
      cb(error, RefmtShared.UnknownLang, RefmtShared.UnknownLang)
    }
  );
  Protocol.Storage.setLatestInput(input);
};

let onOpen = Protocol.OpenInTab.send;

Document.addEventListener(
  "DOMContentLoaded",
  PopupCommon.init(~getSelection, ~getLatestInput, ~onOpen, ~refmt),
);
