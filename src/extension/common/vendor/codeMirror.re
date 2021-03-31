type editor;
[@bs.send] external execCommand : (editor, string) => unit = "execCommand";
// [@bs.module "react-codemirror2"] external codeMirror : ReasonReact.reactClass = "Controlled";
  [@bs.module "react-codemirror2"] [@react.component]
  external make:
    (
      ~autoFocus: option(bool)=?,
      ~style: option(Js.t({..}))=?,
      ~value: option(string)=?,
      ~editorDidMount: option(editor => unit)=?,
      ~onChange: option(string => unit)=?,
      ~options: option(Js.t({..}))=?,
      ~children:React.element=?,
    ) =>
    React.element =
    "Controlled";
// [@bs.module "react-codemirror2"] external codeMirror : ReasonReact.reactClass = "Controlled";
  // [@bs.module "react-codemirror2"] [@react.component]
  // external make:
  //   (
  //     ~value: string,
  //     ~options:option(Js.t({..}))=?,
  //     ~onChange: option(string => unit)=?,
  //     ~onBeforeChange: (~value:'a) =>unit,
  //     ~editorDidMount: editor => unit=?,
  //     ~className: string=?,
  //     ~children:React.element=?,
  //   ) =>
  //   React.element =
  //   "Controlled";


// let make =
//     (
//       ~autoFocus: option(bool)=?,
//       ~style: option(Js.t({..}))=?,
//       ~value: option(string)=?,
//       ~editorDidMount: option(editor => unit)=?,
//       ~onChange: option(string => unit)=?,
//       ~options: option(Js.t({..}))=?,
//       children
//     ) =>
//   ReasonReact.wrapJsForReason(
//     ~reactClass=codeMirror,
//     ~props={
//       "autoFocus": switch (autoFocus) {
//       | Some(true) => true
//       | Some(false) | None => false
//       },
//       "style": Js.Undefined.fromOption(style),
//       "value": Js.Undefined.fromOption(value),
//       "onBeforeChange": (_editor, _data, value) => switch (onChange) {
//       | Some(onChange) => onChange(value)
//       | None => ()
//       },
//       "editorDidMount": Js.Undefined.fromOption(editorDidMount),
//       "options": Js.Undefined.fromOption(options)
//     },
//     children
//   );
