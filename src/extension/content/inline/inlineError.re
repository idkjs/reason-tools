[@react.component]
let make = (~message, ~close) => {
  <div className="root-container">
    <Transition
      before={ReactDOMRe.Style.make(~opacity="0", ())}
      after={ReactDOMRe.Style.make(
        ~opacity="1",
        ~transition="opacity 250ms",
        (),
      )}>
      <div className="mask-container" onClick={_ => close()}>
        <div className="error-message-container">
          {React.string(message)}
        </div>
      </div>
    </Transition>
  </div>;
};
