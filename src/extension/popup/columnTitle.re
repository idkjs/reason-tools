let showVersion = (lang) =>
  switch lang {
  | RefmtShared.RE => Globals.reasonVersion
  | RefmtShared.ML => Globals.ocamlVersion
  | _ => ""
  };

[@react.component]

let make = (~lang, ~select, _) => {
<span title=(showVersion(lang)) style=PopupStyles.contextTitle> select </span>
};
