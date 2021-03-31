type action =
  | Style(ReactDOMRe.Style.t);
[@react.component]
let make = (~before, ~after, ~children) => {
  let (state, dispatch) =
    React.useReducer(
      (_state, action) =>
        switch (action) {
        | Style(a) => a
        },
      before,
    );

  React.useEffect1(
    () => {
      Js.Global.setTimeout(() => {Style(after) |> dispatch}, 0) |> ignore;
      None;
    },
    [|after|],
  );

  <div style=state> {children} </div>;
};
