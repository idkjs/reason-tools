let iconStyle = ReactDOMRe.Style.make(~fontSize="16vh", ());

let textStyle = ReactDOMRe.Style.make(~fontSize="3vh", ~whiteSpace="nowrap", ~marginTop="4vh", ());

[@react.component]
let make = (~show, ~text="Text copied to clipboard", ~style=?) => {
    show ?{
      <div ?style>
        <div style=iconStyle> {React.string(Js.String.fromCodePoint(0x2398))} </div>
        <div style=textStyle> {React.string(text)} </div>
      </div>} :
      {React.null}
};
