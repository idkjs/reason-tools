open Core;

external refToElement: Dom.element => LocalDom.Element.t = "%identity";

type state = {preRef: ref(option(Dom.element))};
type action =
  | UpdateRef(state);
// let updatePreRef = (r, {ReasonReact.state}) =>
//   state.preRef := Js.Nullable.toOption(r);

// let component = ReasonReact.reducerComponent("InlineListing");
[@react.component]
let make = (~lang, ~text, ~slideInFrom, ~open_) => {
  let (state, setState) = React.useState(() => {preRef: ref(None)});
let updatePreRef = (r) =>
  state.preRef := Js.Nullable.toOption(r);
  React.useEffect1(
    () => {
      switch (state.preRef^) {
      | Some(r) => Hljs.highlightBlock(refToElement(r))
      | None => ()
      };
      None;
    },
    [|state|],
  );

  let translateY = slideInFrom == "above" ? "-10vh" : "10vh";
  let className =
    Util.classNames([
      ("listing-container", true),
      ("ml", lang == RefmtShared.ML),
      ("re", lang == RefmtShared.RE),
    ]);
  <Transition
    before={ReactDOMRe.Style.make(
      ~transform={j|translateY($translateY)|j},
      (),
    )}
    after={ReactDOMRe.Style.make(
      ~transform="translateY(0)",
      ~transition="transform 250ms",
      (),
    )}>
    <div className onClick={e => e->ReactEvent.Mouse.stopPropagation}>
      <div className="sidebar">
        {React.string(Protocol.stringOfLanguage(lang))}
      </div>
      <div className="main">
        <pre ref={setState(updatePreRef)}> {React.string(text)} </pre>
        <footer>
          <CopyButton
            text
            style={ReactDOMRe.Style.make(~cursor="pointer", ())}
            onCopy=ignore
          />
          <OpenButton
            style={ReactDOMRe.Style.make(
              ~cursor="pointer",
              ~height="1em",
              ~width="1em",
              ~marginLeft="1em",
              (),
            )}
            onClick={_ => open_(text)}
          />
        </footer>
      </div>
    </div>
  </Transition>;
};
