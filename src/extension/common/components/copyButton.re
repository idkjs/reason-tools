
[@react.component]
let make = (~label="copy", ~text, ~onCopy, ~style=?) => {

    <CopyToClipboard text onCopy>
      <span className="copy-button" ?style> {React.string(label)} </span>
    </CopyToClipboard>
};
