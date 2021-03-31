[@react.component]
let make = (~value, ~autoFocus=?, ~editorDidMount=?, ~lang, ~readOnly=false, ~onChange=?) => {

    <CodeMirror
      value
      ?editorDidMount
      ?autoFocus
      ?onChange
      options={
        "mode": lang == RefmtShared.ML ? "text/x-ocaml" : "javascript",
        "theme": "oceanic-next",
        "readOnly":readOnly
      }
    />
};
