open LocalDom;

open Rebase;

let onOpen: string => unit = [%bs.raw
  {|
  function (hash) {
    window.open(
      window.location.href,
      "_blank"
    );
  }
|}
];

let refmt =
    (
      input,
      ~inLang=RefmtShared.UnknownLang,
      ~inType=RefmtShared.UnknownType,
      ~outLang=RefmtShared.UnknownLang,
      cb
    ) => {
  Protocol.Refmt.send(
    input,
    ~inLang,
    ~inType,
    ~outLang,
    (error) =>
      switch error {
      | Error(error) => cb(error, RefmtShared.UnknownLang, RefmtShared.UnknownLang)
      | Ok({outText, inLang, outLang}) => cb(outText, inLang, outLang)
      }
  );
  Protocol.Storage.setLatestInput(input)
};

Document.addEventListener("DOMContentLoaded", () => PopupCommon.init(~onOpen, ~refmt, ()));
