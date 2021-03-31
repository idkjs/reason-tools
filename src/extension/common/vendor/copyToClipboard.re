// [@bs.module] external copyToClipboard : ReasonReact.reactClass = "react-copy-to-clipboard";

// let make = (~text: string, ~onCopy: unit => unit, children) =>
//   ReasonReact.wrapJsForReason(
//     ~reactClass=copyToClipboard,
//     ~props={"text": text, "onCopy": onCopy},
//     children
//   );
[@bs.module "react-copy-to-clipboard"] [@react.component]
external make:
  (~text: string, ~children: React.element, ~onCopy: unit => unit=?) =>
  React.element =
  "CopyToClipboard";
