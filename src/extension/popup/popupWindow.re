type action =
  | LinkCopyConfirmation
  | TextCopyConfirmation
  | RemoveCopyConfirmation
  | InLanguageChange(RefmtShared.language)
  | OutLanguageChange(RefmtShared.language);

let select = (name, onChange, language, lang) =>
  <select name onChange value=(Protocol.stringOfLanguage(language))>
    <option value="auto">
      (
        React.string(
          "Auto"
          ++ (
            switch (lang, language) {
            | (lang, RefmtShared.UnknownLang) when lang != RefmtShared.UnknownLang =>
              " (" ++ Protocol.stringOfLanguage(lang) ++ ")"
            | _ => ""
            }
          )
        )
      )
    </option>
    <option value="RE"> (React.string("RE")) </option>
    <option value="ML"> (React.string("ML")) </option>
    <option value="REO"> (React.string("RE v1")) </option>
  </select>;

type state = {
  copyConfirmation: string,
  inLanguage: Protocol.language,
  outLanguage: Protocol.language,
  dialogKillTimer: ref(option(Js.Global.timeoutId))
};

let resetTimer = ({ReasonReact.state, reduce}) => {
  switch state.dialogKillTimer^ {
  | None => ()
  | Some(timer) => Js.Global.clearTimeout(timer)
  };
  state.dialogKillTimer := Some(Js.Global.setTimeout(reduce(() => RemoveCopyConfirmation), 2500))
};

let component = ReasonReact.reducerComponent("PopupWindow");

let make =
    (
      ~inText,
      ~inLang,
      ~outText,
      ~outLang,
      ~link,
      ~onOpen,
      ~onInputChanged:
         (~inLang: Protocol.language=?, ~outLang: Protocol.language=?, string) => unit,
      _
    ) => {
  ...component,
  reducer: (action, state) =>
    switch action {
    | LinkCopyConfirmation =>
      ReasonReact.UpdateWithSideEffects(
        {...state, copyConfirmation: "Link copied to clipboard"},
        resetTimer
      )
    | TextCopyConfirmation =>
      ReasonReact.UpdateWithSideEffects(
        {...state, copyConfirmation: "Text copied to clipboard"},
        resetTimer
      )
    | RemoveCopyConfirmation => ReasonReact.Update({...state, copyConfirmation: ""})
    | InLanguageChange(lang) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, inLanguage: lang},
        ((self) => onInputChanged(~inLang=lang, ~outLang=self.state.outLanguage, inText))
      )
    | OutLanguageChange(lang) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, outLanguage: lang},
        ((self) => onInputChanged(~inLang=self.state.inLanguage, ~outLang=lang, inText))
      )
    },
  initialState: () => {
    copyConfirmation: "",
    inLanguage: RefmtShared.UnknownLang,
    outLanguage: RefmtShared.UnknownLang,
    dialogKillTimer: ref(None)
  },
  render: ({state, reduce, handle}) => {
    Js.log(inText);
    let inLanguageChange = (event) => {
      let lang =
        event
        |> ReactEventRe.Synthetic.target
        |> LocalDom.Element.value
        |> Protocol.languageOfString;
      InLanguageChange(lang)
    };
    let outLanguageChange = (event) => {
      let lang =
        event
        |> ReactEventRe.Synthetic.target
        |> LocalDom.Element.value
        |> Protocol.languageOfString;
      OutLanguageChange(lang)
    };
    let handleInputChange = (input, {ReasonReact.state}) =>
      onInputChanged(~inLang=state.inLanguage, ~outLang=state.outLanguage, input);
    <div style=PopupStyles.popup>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=inLang
            select=(select("in", reduce(inLanguageChange), state.inLanguage, inLang))
          />
        </h1>
        <Editor
          value=inText
          lang=inLang
          autoFocus=true
          editorDidMount={editor => CodeMirror.execCommand(editor, "selectAll")}
          onChange=(handle(handleInputChange))
        />
      </div>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=outLang
            select=(select("out", reduce(outLanguageChange), state.outLanguage, outLang))
          />
          <CopyButton
            style=PopupStyles.contextLink
            label="share"
            text=link
            onCopy=(reduce((_) => LinkCopyConfirmation))
          />
          <CopyButton
            style=PopupStyles.contextLink
            text=outText
            onCopy=(reduce((_) => TextCopyConfirmation))
          />
          <OpenButton style=PopupStyles.contextIcon onClick=((_) => onOpen(inText)) />
        </h1>
        <Editor value=outText lang=outLang readOnly=true />
        <CopyConfirmation
          style=PopupStyles.copyConfirmation
          show=(state.copyConfirmation !== "")
          text=state.copyConfirmation
        />
      </div>
    </div>
  }
};
