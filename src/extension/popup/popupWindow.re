type action =
  | LinkCopyConfirmation
  | TextCopyConfirmation
  | RemoveCopyConfirmation
  | InLanguageChange(RefmtShared.language)
  | OutLanguageChange(RefmtShared.language);

let select = (name, onChange, language, lang) =>
  <select name onChange value={Protocol.stringOfLanguage(language)}>
    <option value="auto">
      {React.string(
         "Auto"
         ++ (
           switch (lang, language) {
           | (lang, RefmtShared.UnknownLang)
               when lang != RefmtShared.UnknownLang =>
             " (" ++ Protocol.stringOfLanguage(lang) ++ ")"
           | _ => ""
           }
         ),
       )}
    </option>
    <option value="RE"> {React.string("RE")} </option>
    <option value="ML"> {React.string("ML")} </option>
    <option value="REO"> {React.string("RE v1")} </option>
  </select>;

type state = {
  copyConfirmation: string,
  inLanguage: Protocol.language,
  outLanguage: Protocol.language,
  dialogKillTimer: ref(option(Js.Global.timeoutId)),
};

let initialState = {
  copyConfirmation: "",
  inLanguage: RefmtShared.UnknownLang,
  outLanguage: RefmtShared.UnknownLang,
  dialogKillTimer: ref(None),
};

[@react.component]
let make =
    (
      ~inText,
      ~inLang,
      ~outText,
      ~outLang,
      ~link,
      ~onOpen,
      ~onInputChanged:
         (
           ~inLang: Protocol.language=?,
           ~outLang: Protocol.language=?,
           string
         ) =>
         unit,
      _,
    ) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | LinkCopyConfirmation => {
            ...state,
            copyConfirmation: "Link copied to clipboard",
          }

        | TextCopyConfirmation => {
            ...state,
            copyConfirmation: "Text copied to clipboard",
          }
        | RemoveCopyConfirmation => {...state, copyConfirmation: ""}
        | InLanguageChange(lang) => {...state, inLanguage: lang}

        | OutLanguageChange(lang) => {...state, outLanguage: lang}
        },
      initialState,
    );

  let resetTimer = () => {
    switch (state.dialogKillTimer^) {
    | None => ()
    | Some(timer) => Js.Global.clearTimeout(timer)
    };
    state.dialogKillTimer :=
      Some(Js.Global.setTimeout((() => dispatch(RemoveCopyConfirmation)), 2500));
  };

  Js.log(inText);
  let linkCopyConfirmation = _event => {
    resetTimer();
    dispatch(LinkCopyConfirmation);
  };
  let textCopyConfirmation = _event => {
    resetTimer();
    dispatch(TextCopyConfirmation);
  };
  let inLanguageChange = event => {
    let lang =
      event
      -> ReactEvent.Synthetic.target##value
      |> LocalDom.Element.value
      |> Protocol.languageOfString;
    onInputChanged(~inLang=state.inLanguage, ~outLang=lang, inText);
    InLanguageChange(lang)->dispatch;
  };
  let outLanguageChange = event => {
    let lang =
      event
      -> ReactEvent.Synthetic.target##value
      |> LocalDom.Element.value
      |> Protocol.languageOfString;
    onInputChanged(~inLang=state.inLanguage, ~outLang=lang, inText);
    dispatch(OutLanguageChange(lang));
  };
  let handleInputChange = input =>
    onInputChanged(
      ~inLang=state.inLanguage,
      ~outLang=state.outLanguage,
      input,
    );
  <div style=PopupStyles.popup>
    <div style=PopupStyles.popupColumn>
      <h1 style=PopupStyles.popupContext>
        <ColumnTitle
          lang=inLang
          select={select(
            "in",
            inLanguageChange,
            state.inLanguage,
            inLang,
          )}
        />
      </h1>
      <Editor
        value=inText
        lang=inLang
        autoFocus=true
        editorDidMount={editor => CodeMirror.execCommand(editor, "selectAll")}
        onChange={text => handleInputChange(text)}
      />
    </div>
    <div style=PopupStyles.popupColumn>
      <h1 style=PopupStyles.popupContext>
        <ColumnTitle
          lang=outLang
          select={select(
            "out",
            outLanguageChange,
            state.outLanguage,
            outLang,
          )}
        />
        <CopyButton
          style=PopupStyles.contextLink
          label="share"
          text=link
          onCopy={_ => linkCopyConfirmation()}
        />
        <CopyButton
          style=PopupStyles.contextLink
          text=outText
          onCopy={_ => textCopyConfirmation()}
        />
        <OpenButton
          style=PopupStyles.contextIcon
          onClick={_ => onOpen(inText)}
        />
      </h1>
      <Editor value=outText lang=outLang readOnly=true />
      <CopyConfirmation
        style=PopupStyles.copyConfirmation
        show={state.copyConfirmation !== ""}
        text={state.copyConfirmation}
      />
    </div>
  </div>;
};
