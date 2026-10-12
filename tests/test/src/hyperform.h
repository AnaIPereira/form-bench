#include- declare-hyperform.h
#include- mzvlow.h
#procedure HypAppendToPreprocList(PreprocListVar,ItemVar)
  #If ("``ItemVar''" == "")
    #Write "Empty new item is not allowed to be appended..."
    #Write "Item: `ItemVar' = >>>``ItemVar''<<<"
    #Terminate
  #EndIf
  #Do Item={``ItemVar'',}
    #If ("`Item'" != "")
      #If ("``PreprocListVar''" != "")
        #Redefine `PreprocListVar' "``PreprocListVar'',`Item'"
      #Else
        #Redefine `PreprocListVar' "`Item'"
      #EndIf
    #EndIf
  #EndDo
#endprocedure
#procedure HypIncrementPreprocCounter(PreprocVar)
  #redefine `PreprocVar' "{``PreprocVar''+1}"
#endprocedure
#procedure HypCountListItems(CountVar,?list)
  #Redefine `CountVar' "0"
  #If ("`?list'" != "")
    #Do item={`?list',}
      #If ("`item'" != "")
        #Redefine `CountVar' "{``CountVar''+1}"
      #EndIf
    #EndDo
  #EndIf
#endprocedure
#procedure HypFindItemLocationInList(ItemVar,PosVar,?list)
  #Redefine `PosVar' "0"
  #Define TemporaryPosition "0"
  #Do tmp={`?list',}
    #If ("`tmp'" != "")
      #Redefine TemporaryPosition "{`TemporaryPosition'+1}"
    #EndIf
    #If ("`tmp'" == "``ItemVar''")
      #Redefine `PosVar' "`TemporaryPosition'"
      #BreakDo
    #EndIf
  #EndDo
#endprocedure
#procedure HypGetBitValueForVar(Var,BitValueVar,?vars)
  #Define VariablePosition ""
  #Define TempItem "`Var'"
  #call HypFindItemLocationInList(TempItem,VariablePosition,`?vars')
  #If (`VariablePosition' == 0)
    #write "`Var' cannot be found among the variables: `?vars'"
    #write "HypGetBitValueForVar"
    #terminate
  #Else
    #Define NumberOfVars ""
    #call HypCountListItems(NumberOfVars,`?vars')
    #Redefine `BitValueVar' "{2^(`NumberOfVars'-`VariablePosition')}"
  #EndIf
#endprocedure
#procedure HypHideActiveExpressions(?SkippedExprs)
  #If ("`HYPactiveExpressions'" != "")
    #Write "HYPactiveExpressions is not empty..."
    #Write "`HYPactiveExpressions'"
    #Terminate
  #Else
    #Redefine HYPactiveExpressions "`ACTIVEEXPRNAMES_'"
    Hide;
    #If ("`?SkippedExprs'" != "")
      nHide `?SkippedExprs';
    #EndIf
  #EndIf
#endprocedure
#procedure HypUnhideActiveExpressions
  UnHide `HYPactiveExpressions';
  #Redefine HYPactiveExpressions ""
#endprocedure
#procedure HypCountTermsInExpression(ExprID,CountFuncID,dCount)
  #If ("`ActiveExprNames_'" != "`ExprID'")
    #Write "We need to have only `ExprID' active..."
    #Write "Instead, we found: `ActiveExprNames_'"
    #Write "HypCountTermsInExpression"
    #Terminate
  #EndIf
  #$`dCount' = 0;
  $`dCount' = $`dCount' + 1;
  Multiply `CountFuncID'($`dCount');
  ModuleOption,NoParallel;
  .sort:`ExprID' counted;
#endprocedure
#procedure HypExpressionToDynamicSetInFile(ExprID,DumpFileName,SetFileName)
  Format NoSpaces;
  #write <`DumpFileName'> "%E",`ExprID'
  #close <`DumpFileName'>
  #write <`SetFileName'> "{"
  #close <`SetFileName'>
  #system sed -e "s/+/,/g" `DumpFileName' >> `SetFileName'
  #append <`SetFileName'>
  #write <`SetFileName'> "}"
  #close <`SetFileName'>
  .sort:`ExprID' to `SetFileName';
  Format;
#endprocedure
#procedure HypFunctionArgsToDynamicSetInFile(ExprID,FuncID,DumpFileName,SetFileName)
  Format NoSpaces;
  #write <`DumpFileName'> "%E",`ExprID'
  #close <`DumpFileName'>
  #write <`SetFileName'> "{"
  #close <`SetFileName'>
  #system tr -d " \n" < `DumpFileName' | sed -e "s/`FuncID'(\(-\{0,1\}[0-9]*\))/\1/g" -e "s/+/,/g" >> `SetFileName'
  #append <`SetFileName'>
  #write <`SetFileName'> "}"
  #close <`SetFileName'>
  .sort:`ExprID' to `SetFileName';
  Format;
#endprocedure
#procedure HypStoreExpressionInTableWithTag(ExprID,TagFuncID,TableID,DropExpr)
  Bracket `TagFuncID';
  .sort:`ExprID' bracketed with `TagFuncID';
  FillExpression `TableID' = `ExprID'(`TagFuncID');
  #If (`DropExpr' == 1)
    .sort:`TableID' filled with `ExprID';
    Drop `ExprID';
  #EndIf
#endprocedure
#procedure HypDistinctFuncsToExpression(ExprIDvar,FuncID,WrapFuncID,IndexFuncID,AuxFuncID,SymbolID,TempExprPrefix,TempExprCountVar,HiddenExprsVar,?ArgCases)
  #call HypStartArgCases(`?ArgCases')
  id `FuncID'(?a) = `WrapFuncID'(`FuncID'(?a));
  #call HypFinishArgCases(`?ArgCases')
  #Define HYPtempOldExtras ""
  #Define HYPtempNewExtras ""
  #call HypWrappedArgsToExtraSymbolNumbers(`WrapFuncID',`IndexFuncID',HYPtempOldExtras,HYPtempNewExtras,`SymbolID',`?ArgCases')
  #Define HYPtempDropExtras "1"
  #Redefine `HiddenExprsVar' "`ActiveExprNames_'"
  Hide;
  #call HypExtraSymbolNumbersToExpressionLite(`ExprIDvar',`IndexFuncID',`HYPtempOldExtras',`HYPtempNewExtras',`TempExprPrefix',`TempExprCountVar',`HYPtempDropExtras')
#endprocedure
#procedure HypParseInputExpr(epID,numID,denID,?params)
  id exp_(HYPn1?,HYPn2?) = HYPpow(HYPn1,HYPn2);
  id `denID'(HYPn1?) = HYPpow(HYPn1,-1);
  id `numID'(HYPn1?) = HYPpow(HYPn1,1);
  #do par={`?params'}
    id `par' = HYPpow(`par',1);
  #enddo
  multiply replace_(`epID',HYPep);
  #define iPar "0"
  #do par={`?params'}
    #redefine iPar "{`iPar'+1}"
    multiply replace_(`par',HYPz`iPar');
  #enddo
  #if (`iPar' > `HYPNPARAMS')
    #redefine HYPNPARAMS "`iPar'"
    #write "Number of hyper parameters increased to `HYPNPARAMS'"
  #endif
  #call HypPowFuncSimplify
#endprocedure
#procedure HypArgsToRatFunc(ExprID,FuncID,RatFuncID,AuxFuncID,n1)
  .sort:ArgsToRatFunc start;
  skip;
  nskip `ExprID';
  #$ArgsToRatFuncEXTRASYM = `extrasymbols_';
  argtoextrasymbol,tonumber,`FuncID';
  repeat;
    id `FuncID'(?a,`n1'?number_,?b) = `FuncID'(?a,`AuxFuncID'(`n1'),?b);
  endrepeat;
  .sort:Args wrapped;
  skip;
  nskip `ExprID';
  argument `FuncID';
    id `AuxFuncID'(`n1'?number_) = `AuxFuncID'(extrasymbol_(`n1'));
  endargument;
  .sort:Args subst back;
  skip;
  #if (`$ArgsToRatFuncEXTRASYM' > 0)
    delete extrasymbols>`$ArgsToRatFuncEXTRASYM';
  #else
    delete extrasymbols;
  #endif
  .sort:Back to normal args;
  skip;
  nskip `ExprID';
  argument `FuncID';
    id `AuxFuncID'(`RatFuncID'(?a)) = `RatFuncID'(?a);
    id `AuxFuncID'(`n1'?) = `RatFuncID'(`n1',1);
  endargument;
#endprocedure
#procedure HypCreateRatFuncArgsInVar(ExprID,FuncID,UseFilter,FilterFuncID,ArgFuncID,PolyFuncID,Var,AuxFuncID,Sym1,Sym2)
  .sort:CreateRatFuncArgsInVar start;
  skip;
  nskip `ExprID';
  if (
  #if (`UseFilter')
    count(`FilterFuncID',1) > 0
  #else
    1
  #endif
  );
    repeat;
      id `FuncID'(?a,`Sym1'?number_,?b) = `FuncID'(?a,`ArgFuncID'(`Sym1', 1), ?b);
    endrepeat;
    argument `FuncID';
      if (match(`ArgFuncID'(`PolyFuncID'(?a),`PolyFuncID'(?b))) = 0);
        id `ArgFuncID'(`Sym1'?,`Sym2'?) = `ArgFuncID'(`PolyFuncID'(`Sym1'),`PolyFuncID'(`Sym2'));
        argument `ArgFuncID';
          #call HypGetPolyCoeffs(`PolyFuncID',`Sym1',`Var',1)
        endargument;
        #call HypSyncNumDenOrder(`ArgFuncID',`PolyFuncID',`PolyFuncID',`AuxFuncID',`Sym1',`Sym2')
      endif;
    endargument;
  endif;
#endprocedure
#procedure HypStartArgCases(?ArgCases)
  #Do ArgCase={`?ArgCases',}
    #If ("`ArgCase'" != "")
      #Do Arg={`ArgCase',}
        #If ("`Arg'" != "")
          Argument `Arg';
        #EndIf
      #EndDo
    #EndIf
  #EndDo
#endprocedure
#procedure HypFinishArgCases(?ArgCases)
  #Do ArgCase={`?ArgCases',}
    #If ("`ArgCase'" != "")
      #Do Arg={`ArgCase',}
        #If ("`Arg'" != "")
          EndArgument;
        #EndIf
      #EndDo
    #EndIf
  #EndDo
#endprocedure
#procedure HypDropIndepTerms(ExprID,FuncID,ArgFuncID,Var,FilterFuncID,CheckPolys,PolyFuncID,n1,n2)
  .sort:DropIndepTerms start;
  skip;
  nskip `ExprID';
  if (count(`FilterFuncID',1) == 0);
    $varmax = 0;
    argument `FuncID';
      if (count(`Var',1) > 0) $varmax = count_(`Var',1);
      argument `ArgFuncID';
        if (count(`Var',1) > 0) $varmax = count_(`Var',1);
        #if (`CheckPolys')
          if (match(`PolyFuncID'(`n1'?,?a,`n2'?!{0,},?b)) > 0) $varmax = 1;
        #endif
      endargument;
    endargument;
    if ($varmax == 0) discard;
  endif;
  ModuleOption,local $varmax;
  .sort:DropIndepTerms fin;
#endprocedure
#procedure HypIncludeTagInExprsWhenFunctionPresent(TagFuncID,FuncID,?Exprs)
  .sort:IncludeTagInExprsWhenFunctionPresent start;
  #define ActivesIncludeTagInExprs "`ActiveExprNames_'"
  hide;
  .sort:Hidden everything;
  unhide `?Exprs';
  .sort:Unhidden `?Exprs';
  if (count(`FuncID',1) > 0) multiply `TagFuncID';
  .sort:`?Exprs' tagged;
  hide;
  .sort:Everything got hidden;
  unhide `ActivesIncludeTagInExprs';
  .sort:Restored original actives, fin;
#endprocedure
#procedure HypIncrementZeroedOutCounter(NewZerosVar)
  #redefine HYPzeroedOutTotal "{`HYPzeroedOutTotal'+``NewZerosVar''}"
#endprocedure
#procedure HypIsZeroAtInfinityLimit(Var,NumFuncID,DenFuncID,DegSymbol,dIsZero)
  If ((match(`NumFuncID'(?a,0)) > 0) || (match(`DenFuncID'(?a,0)) > 0));
    Print "Predicate only works if the last argument of `NumFuncID' and `DenFuncID' are non-zero";
    Print "Term: %t";
    Print "HypIsZeroAtInfinityLimit";
    Exit;
  EndIf;
  id `Var' = `Var'*`DegSymbol';
  id 1/`Var' = 1/(`Var'*`DegSymbol');
  id `NumFuncID'(?a) = `NumFuncID'(?a)*`DegSymbol'^(nargs_(?a)-1);
  id `DenFuncID'(?a) = `DenFuncID'(?a)*`DegSymbol'^(1-nargs_(?a));
  If (count(`DegSymbol',1) >= 0);
    $`dIsZero' = 0;
  Else;
    $`dIsZero' = 1;
  EndIf;
  Multiply Replace_(`DegSymbol',1);
  ModuleOption,Local,$`dIsZero';
#endprocedure
#procedure HypIsZeroAtZeroLimit(Var,NumFuncID,DenFuncID,LfuncID,AuxFuncID,DegSymbol,SymbolID,dIsZero)
  id `Var' = `Var'*`DegSymbol';
  id 1/`Var' = 1/(`Var'*`DegSymbol');
  id `NumFuncID'(?a,`SymbolID'?!{0,},?b) = `NumFuncID'(?a,`SymbolID',?b)*`AuxFuncID'(?a,1);
  Repeat id `AuxFuncID'(?a,`SymbolID'?!{0,},?b,1) = `AuxFuncID'(?a,1);
  id `AuxFuncID'(?a) = `DegSymbol'^(nargs_(?a)-1);
  id `DenFuncID'(?a,`SymbolID'?!{0,},?b) = `DenFuncID'(?a,`SymbolID',?b)*`AuxFuncID'(?a,1);
  Repeat id `AuxFuncID'(?a,`SymbolID'?!{0,},?b,1) = `AuxFuncID'(?a,1);
  id `AuxFuncID'(?a) = `DegSymbol'^(1-nargs_(?a));
  If (count(`DegSymbol',1) > 0);
    $`dIsZero' = 1;
  ElseIf (
    (count(`DegSymbol',1) == 0) && 
    (match(`LfuncID'(?a)) > 0) && 
    (match(`LfuncID'(?a,`SymbolID'?!{0,},?b)) > 0)
  );
    $`dIsZero' = 1;
  Else;
    $`dIsZero' = 0;
  EndIf;
  Multiply Replace_(`DegSymbol',1);
  ModuleOption,Local,$`dIsZero';
#endprocedure
#procedure HypMaskFunctionArguments(MaskedArgsFuncID,ArgsFuncID,MaskFuncID,DropZeroArgs,Sym1,Sym2)
  If ((count(`ArgsFuncID',1) == 1) && (count(`MaskFuncID',1) == 1));
    Multiply `MaskedArgsFuncID';
    Repeat;
      id `MaskedArgsFuncID'(?a)*
        `ArgsFuncID'(`Sym1'?,?b)*
        `MaskFuncID'(`Sym2'?,?c) = 
        `MaskedArgsFuncID'(?a,`Sym1'*`Sym2')*`ArgsFuncID'(?b)*`MaskFuncID'(?c)
      ;
      id `ArgsFuncID'*`MaskFuncID' = 1;
    EndRepeat;
    #If (`DropZeroArgs')
      Repeat id `MaskedArgsFuncID'(?a,0,?b) = `MaskedArgsFuncID'(?a,?b);
    #EndIf
  EndIf;
#endprocedure
#procedure HypSymbolsToNumbersInFuncArgWithSetInFile(SetFileID,FuncID,SymbolID)
  Repeat;
    id `SymbolID'1?
        #include- `SetFileID' 
        [`SymbolID'2?]^`SymbolID'3?!{0,}
        = `FuncID'(`SymbolID'2)^`SymbolID'3
      ;
  EndRepeat;
#endprocedure
#procedure HypPullOutSingleTermFactors(FuncID,SymbolID)
  If (match(`FuncID'(`SymbolID'1?,`SymbolID'2?,?a)) > 0);
    Print "For proper operation `FuncID' need to have a single argument...";
    Print "TERM: %t";
    Print "HypPullOutSingleTermFactors";
    Exit;
  EndIf;
  SplitArg `FuncID';
  id `FuncID'(`SymbolID'1?) = `SymbolID'1;
  Transform,`FuncID',AddArgs(1,last);
  FactArg `FuncID';
  ChainOut `FuncID';
  id `FuncID'(`SymbolID'1?number_) = `SymbolID'1;
  id `FuncID'(`SymbolID'1?symbol_) = `SymbolID'1;
  id `FuncID'(1/`SymbolID'1?symbol_) = 1/`SymbolID'1;
#endprocedure
#procedure HypSchwartzZippelSubstitution(IntVariablesListVar,ParametersListVar,SymbolID)
  #Define iPrime "0"
  #Do IntVar={``IntVariablesListVar'',}
    #If ("`IntVar'" != "")
      #call HypIncrementPreprocCounter(iPrime)
      id `IntVar'^`SymbolID'1?!{0,} = prime_(`iPrime')^`SymbolID'1;
    #EndIf
  #EndDo
  #Do Param={``ParametersListVar'',}
    #If ("`Param'" != "")
      #call HypIncrementPreprocCounter(iPrime)
      id `Param'^`SymbolID'1?!{0,} = prime_(`iPrime')^`SymbolID'1;
    #EndIf
  #EndDo
#endprocedure
#procedure HypSchwartzZippelCancel(FuncID,EvalFuncID,AuxFuncID,SymbolID,ExtrasymbolTableID,TempExprPrefix,TempExprCountVar,IntVariablesListVar,ParametersListVar,?ArgCases)
  If (Occurs(`EvalFuncID') > 0);
    Print "`EvalFuncID' is not allowed in expression with this routine...";
    Print "TERM: %t";
    Print "HypSchwartzZippelCancel";
    Exit;
  EndIf;
  #Define HYPtempOldExtras "`Extrasymbols_'"
  #call HypStartArgCases(`?ArgCases')
    Transform,`FuncID',Replace(1,last)=(xarg_,`EvalFuncID'(xarg_));
    Argument `FuncID';
      #call HypPullOutSingleTermFactors(`EvalFuncID',`SymbolID')
      If ((match(`EvalFuncID'(`SymbolID'1?number_)) > 0) || (match(`EvalFuncID'(`SymbolID'1?symbol_)) > 0));
        Print "`EvalFuncID' cannot have single term arguments...";
        Print "TERM: %t";
        Print "HypSchwartzZippelCancel";
        Exit;
      EndIf;
    EndArgument;
  #call HypFinishArgCases(`?ArgCases')
  .sort:`EvalFuncID' introduced;
  #call HypStartArgCases(`?ArgCases')
    Argument `FuncID';
      ArgToExtraSymbol,ToNumber,`EvalFuncID';
      #If (`HYPtempOldExtras' > 0)
        id `EvalFuncID'(`SymbolID'1?{<{`HYPtempOldExtras'+1}}) = `ExtrasymbolTableID'(`SymbolID'1);
      #EndIf
    EndArgument;
  #call HypFinishArgCases(`?ArgCases')
  .sort:Args converted;
  #Define HYPtempActiveExpressions "`ActiveExprNames_'"
  Hide;
  .sort
  #Define HYPtempExtrasExpr ""
  #Define HYPtempDropExtras "0"
  #call HypExtraSymbolNumbersToExpression(HYPtempExtrasExpr,`EvalFuncID',`AuxFuncID',`SymbolID',`HYPtempOldExtras',`Extrasymbols_',`TempExprPrefix',`TempExprCountVar',`HYPtempDropExtras')
  FromPolynomial;
  #call HypSchwartzZippelSubstitution(`IntVariablesListVar',`ParametersListVar',`SymbolID')
  .sort
  DropCoefficient;
  .sort
  #call HypFunctionArgsToDynamicSetInFile(`HYPtempExtrasExpr',`EvalFuncID',sz-dump.expr,sz-set.h)
  .sort
  Unhide `HYPtempActiveExpressions';
  Drop `HYPtempExtrasExpr';
  .sort
  #call HypStartArgCases(`?ArgCases')
    Argument `FuncID';
      id `EvalFuncID'(`SymbolID'1?!
        #include- sz-set.h
        ) = 0;
      id `EvalFuncID'(`SymbolID'1?) = extrasymbol_(`SymbolID'1);
    EndArgument;
  #call HypFinishArgCases(`?ArgCases')
  .sort
  Delete,Extrasymbols>`HYPtempOldExtras';
  .sort:Schwartz-Zippel applied;
#endprocedure
#procedure HypRestoreExtraSymbols(FirstExtra,LastExtra)
  #Do iExtra={`FirstExtra'+1},`LastExtra'
    Multiply Replace_(Z`iExtra'_,extrasymbol_(`iExtra'));
  #EndDo
#endprocedure
#procedure HypFillVarDepTable(TempExprPrefix,TempExprCountVar,FirstExtra,LastExtra,MarkFuncID,VarDepTableID,VarsListVar,NumberOfVarDepFilledVar,Sym1,Sym2)
  #Define NumberOfVars ""
  #call HypCountListItems(NumberOfVars,``VarsListVar'')
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  local `TempExprPrefix'``TempExprCountVar'' =
    #Do iExtra={`FirstExtra'+1},`LastExtra'
      #$TMP = extrasymbol_(`iExtra');
      #Inside $TMP
        FromPolynomial;
        DropCoefficient;
        id `Sym1'?!{``VarsListVar''} = 1;
        id 1/`Sym1'?!{``VarsListVar''} = 1;
        id 1/`Sym1'? = `Sym1';
        If (count(denom_,1) > 0);
          Print "Cannot determine variable dependence in denominator function...";
          Print "HypFillVarDepTable";
          Exit;
        EndIf;
        Repeat id `Sym1'?^2 = `Sym1';
        Repeat id `Sym1'?*`Sym2'? = `Sym1' + `Sym2';
        If (match(`Sym1'?{``VarsListVar''}) == 0) discard;
      #EndInside
      #Inside $TMP
        Multiply 1/coeff_;
        id `Sym1'?{``VarsListVar''}[`Sym2'] = 2^(`NumberOfVars'-`Sym2');
      #EndInside
      + `$TMP'*`MarkFuncID'(`iExtra')
    #EndDo
  ;
  .sort:`TempExprPrefix'``TempExprCountVar'' created for extras;
  Skip;
  nSkip `TempExprPrefix'``TempExprCountVar'';
  Bracket `MarkFuncID';
  .sort
  #Redefine `NumberOfVarDepFilledVar' "`LastExtra'"
  #Do iExtra={`FirstExtra'+1},`LastExtra'
    #If (`iExtra' > `HYPextraSymTblFilled')
      Fill HYPextraSymTbl(`iExtra') = Z`iExtra'_;
      #Redefine HYPextraSymTblFilled "`iExtra'"
    #EndIf
  #EndDo
  FillExpression `VarDepTableID' = `TempExprPrefix'``TempExprCountVar''(`MarkFuncID');
  .sort
  Drop `TempExprPrefix'``TempExprCountVar'';
#endprocedure
#procedure HypPolyArgsToExtraSymbols(FuncID,FactFuncID,WrapFuncID,FirstArg,LastArg,OldExtraCountVar,NewExtraCountVar,Sym1,Sym2,?ArgFunctions)
  #Redefine `OldExtraCountVar' "`ExtraSymbols_'"
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      Argument `Arg';
    #EndIf
  #EndDo
  Transform,`FuncID',replace(`FirstArg',`LastArg')=(xarg_,`FactFuncID'(xarg_));
  Argument `FuncID';
    FactArg `FactFuncID';
    Transform,`FactFuncID',replace(1,last)=(xarg_,`WrapFuncID'(xarg_));
  EndArgument;
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      EndArgument;
    #EndIf
  #EndDo
  .sort:`FuncID' args factored;
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      Argument `Arg';
    #EndIf
  #EndDo
  Argument `FuncID';
    Argument `FactFuncID';
      id `WrapFuncID'(`Sym1'?number_) = `Sym1';
      id `WrapFuncID'(`Sym1'?symbol_) = `Sym1';
      id `WrapFuncID'(`Sym1'?) = `WrapFuncID'(nterms_(`Sym1'),`Sym1');
      id `WrapFuncID'(1,`Sym1'?) = `Sym1';
      id `WrapFuncID'(`Sym1'?{>1},`Sym2'?) = `WrapFuncID'(`Sym2');
      ArgToExtraSymbol `WrapFuncID';
      id `WrapFuncID'(`Sym1'?symbol_) = `Sym1';
    EndArgument;
    Transform,`FactFuncID',mulargs(1,last);
    id `FactFuncID'(`Sym1'?) = `Sym1';
  EndArgument;
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      EndArgument;
    #EndIf
  #EndDo
  .sort:`FuncID' args to extras;
  #Redefine `NewExtraCountVar' "{`ExtraSymbols_'-``OldExtraCountVar''}"
#endprocedure
#procedure HypEvalExtraSymbolArgs(EvalFuncID,OldExtraCountVar,NewExtraCountVar,Sym1,Sym2,?ArgFunctions)
  #Redefine `OldExtraCountVar' "`ExtraSymbols_'"
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      Argument `Arg';
    #EndIf
  #EndDo
  SplitArg `EvalFuncID';
  id `EvalFuncID'(`Sym1'?) = `Sym1';
  Transform,`EvalFuncID',AddArgs(1,last);
  FactArg `EvalFuncID';
  Repeat;
    id `EvalFuncID'(?a,`Sym1'?number_,?b) = `Sym1'*`EvalFuncID'(?a,?b);
    id `EvalFuncID'(?a,`Sym1'?symbol_,?b) = `Sym1'*`EvalFuncID'(?a,?b);
    id `EvalFuncID'(?a,1/`Sym1'?symbol_,?b) = 1/`Sym1'*`EvalFuncID'(?a,?b);
    id `EvalFuncID' = 1;
  EndRepeat;
  Argument `EvalFuncID';
    FromPolynomial;
  EndArgument;
  FactArg `EvalFuncID';
  Repeat;
    id `EvalFuncID'(?a,`Sym1'?number_,?b) = `Sym1'*`EvalFuncID'(?a,?b);
    id `EvalFuncID'(?a,`Sym1'?symbol_,?b) = `Sym1'*`EvalFuncID'(?a,?b);
    id `EvalFuncID'(?a,1/`Sym1'?symbol_,?b) = 1/`Sym1'*`EvalFuncID'(?a,?b);
    id `EvalFuncID' = 1;
  EndRepeat;
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      EndArgument;
    #EndIf
  #EndDo
  .sort:After FromPoly in `EvalFuncID';
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      Argument `Arg';
    #EndIf
  #EndDo
  ArgToExtraSymbol `EvalFuncID';
  Transform,`EvalFuncID',mulargs(1,last);
  id `EvalFuncID'(`Sym1'?) = `Sym1';
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      EndArgument;
    #EndIf
  #EndDo
  .sort:ArgToExtra in `EvalFuncID';
  #Redefine `NewExtraCountVar' "{`ExtraSymbols_' - ``OldExtraCountVar''}"
#endprocedure
#procedure HypPrintExtraSymbolTable(FirstExtra,LastExtra)
  #If (`LastExtra' > `FirstExtra')
    #write "Printing extra symbols in the range of [{`FirstExtra'+1},`LastExtra']"
    #Do iExtra={`FirstExtra'+1},`LastExtra'
      #$TMP = extrasymbol_(`iExtra');
      #write "Z`iExtra'_ = %$",$TMP;
    #EndDo
  #EndIf
#endprocedure
#procedure HypCheckVarDependenceThruGlobalDollar(Var,dCheckVar,ExtraSymbolPosition,MaskFuncID,VarsFuncID,AuxFuncID,VarDepTableID,NumberOfVarsVar,VarsListVar,Sym1,Sym2)
  #$`dCheckVar' = `MaskFuncID'(`VarDepTableID'(`ExtraSymbolPosition'));
  #Inside $`dCheckVar'
    Transform,`MaskFuncID',Decode(1,``NumberOfVarsVar''):base=2;
    Multiply `VarsFuncID'(``VarsListVar'');
    #call HypMaskFunctionArguments(`AuxFuncID',`VarsFuncID',`MaskFuncID',1,`Sym1',`Sym2')
    If (occurs(`Var') == 0);
      discard;
    Else;
      id `AuxFuncID'(?a) = 1;
    EndIf;
  #EndInside
#endprocedure
#procedure HypCheckVarDependenceThruLocalDollar(Var,dExtraSymbolPosition,AuxFuncID,VarDepTableID,NumberOfVarsVar,VarsListVar,dDep,SymbolID1,SymbolID2)
  $`dDep' = `AuxFuncID'1(`VarDepTableID'($`dExtraSymbolPosition'))*`AuxFuncID'2(``VarsListVar'');
  ModuleOption,Local,$`dDep';
  Inside $`dDep';
    id `AuxFuncID'1(`SymbolID1'?)*`AuxFuncID'2(?a,`Var',?b) = 
      `AuxFuncID'1(`SymbolID1')*
      `AuxFuncID'2(`SymbolID1'-2^nargs_(?b))*
      `AuxFuncID'3(nargs_(?a)+1);
    If (match(`AuxFuncID'2(`SymbolID1'?,`SymbolID2'?{<0})) > 0) Discard;
    Transform,`AuxFuncID'1,Decode(1,``NumberOfVarsVar''):base=2;
    id `AuxFuncID'1(?a)*`AuxFuncID'3(`SymbolID1'?) = PutFirst_(`AuxFuncID'1,`SymbolID1',?a);
    If (match(`AuxFuncID'1(0,?a)) > 0) Discard;
  EndInside;
  $`dDep' = thetap_(nterms_($`dDep'));
#endprocedure
#procedure HypRestoreExtraSymbolsByVar(Var,PolyFuncID,VarDepTableID,VarsListVar,NumberOfVarDepFilledVar,AuxFuncID,MaskFuncID,VarsFuncID,Sym1,Sym2)
  #If (`ExtraSymbols_' != ``NumberOfVarDepFilledVar'')
    #write "More extra symbols than filled entries in `VarDepTableID'"
    #write "Number of extras:  `ExtraSymbols_'"
    #write "Number of entries: ``NumberOfVarDepFilledVar''"
    #write "HypRestoreExtraSymbolsByVar"
    #terminate
  #EndIf
  #Define NumberOfVars ""
  #call HypCountListItems(NumberOfVars,``VarsListVar'')
  #Do iExtra=1,`ExtraSymbols_'
    #call HypCheckVarDependenceThruGlobalDollar(`Var',VarDepCheck,`iExtra',`MaskFuncID',`VarsFuncID',`AuxFuncID',`VarDepTableID',NumberOfVars,`VarsListVar',`Sym1',`Sym2')
    #If (TermsIn($VarDepCheck) > 0)
      Multiply replace_(Z`iExtra'_,`PolyFuncID'(ExtraSymbol_(`iExtra')));
    #EndIf
  #EndDo
#endprocedure
#procedure HypRestoreExtraSymbolsAsCoeffsByVar(Var,PolyFuncID,VarDepTableID,VarsListVar,NumberOfVarDepFilledVar,TempExprPrefix,TempExprCountVar,FactFuncID,WrapFuncID,AuxFuncID,MaskFuncID,TagFuncID,VarsFuncID,SubstitutionTableID,Sym1,Sym2,Sym3,?ArgCases)
  #If (`ExtraSymbols_' != ``NumberOfVarDepFilledVar'')
    #write "More extra symbols than filled entries in `VarDepTableID'"
    #write "Number of extras:  `ExtraSymbols_'"
    #write "Number of entries: ``NumberOfVarDepFilledVar''"
    #write "HypRestoreExtraSymbolsAsCoeffsByVar"
    #terminate
  #EndIf
  #Define NumberOfVars ""
  #call HypCountListItems(NumberOfVars,``VarsListVar'')
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  PushHide;
  local `TempExprPrefix'``TempExprCountVar'' =
    #If (`ExtraSymbols_' > 0)
      <Z1_*`MaskFuncID'(1)> + ... + <Z`ExtraSymbols_'_*`MaskFuncID'(`ExtraSymbols_')>
    #EndIf
  ;
  id `MaskFuncID'(`Sym1'?$ExtraPos) = 1;
  ModuleOption,Local,$ExtraPos;
  #call HypCheckVarDependenceThruLocalDollar(`Var',ExtraPos,`AuxFuncID',`VarDepTableID',NumberOfVars,`VarsListVar',ExtraDep,`Sym1',`Sym2')
  If ($ExtraDep == 0) Discard;
  .sort:`TempExprPrefix'``TempExprCountVar'' created for extras;
  #call HypExpressionToDynamicSetInFile(`TempExprPrefix'``TempExprCountVar'',listdump.txt,subst.h)
  #call HypCountTermsInExpression(`TempExprPrefix'``TempExprCountVar'',`TagFuncID',ExtraCount)
  id `Sym1'?symbol_ = `PolyFuncID'(`Sym1');
  Argument `PolyFuncID';
    FromPolynomial;
  EndArgument;
  #call HypGetPolyCoefficients(`PolyFuncID',`Var',`Sym1')
  #Define OldExtras "`ExtraSymbols_'"
  #Define NewExtras ""
  #call HypPolyArgsToExtraSymbols(`PolyFuncID',`FactFuncID',`WrapFuncID',1,last,OldExtras,NewExtras,`Sym1',`Sym2',)
  .sort
  #call HypStoreExpressionInTableWithTag(`TempExprPrefix'``TempExprCountVar'',`TagFuncID',`SubstitutionTableID',1)
  PopHide;
  #If ("`?ArgCases'" == "")
    Repeat;
      id `Sym1'?
        #include- subst.h
        [`Sym2'?]^`Sym3'?!{0,}
        = `TagFuncID'(`Sym2')^`Sym3'
      ;
    EndRepeat;
  #EndIf
  #Do ArgFunc={`?ArgCases',}
    #If ("`ArgFunc'" != "")
      #Do Arg={``ArgFunc'',}
        #If ("`Arg'" != "")
          Argument `Arg';
        #EndIf
      #EndDo
      Repeat;
        id `Sym1'?
          #include- subst.h 
          [`Sym2'?]^`Sym3'?!{0,}
          = `TagFuncID'(`Sym2')^`Sym3'
        ;
      EndRepeat;
      #Do Arg={``ArgFunc'',}
        #If ("`Arg'" != "")
          EndArgument;
        #EndIf
      #EndDo
    #EndIf
  #EndDo
  Multiply Replace_(`TagFuncID',`SubstitutionTableID');
  .sort:Back-substituted;
  ClearTable `SubstitutionTableID';
  .sort `SubstitutionTableID' cleared;
#endprocedure
#procedure HypDetectSqrtAndConvert(FuncID,SFuncID,SqrtFuncID,AuxFuncID,Sym1,Sym2)
  id `FuncID'(`Sym1'?,`Sym2'?,?a) = `AuxFuncID'(`Sym1',`Sym2',?a);
  SplitArg,((`SqrtFuncID')),`FuncID';
  Transform,`FuncID',AddArgs(2,last);
  id `FuncID'(`Sym1'?,`Sym2'?) = `SFuncID'(`AuxFuncID'(`Sym2'),`Sym1');
  id `FuncID'(`SqrtFuncID'(`Sym1'?)) = `SFuncID'(`AuxFuncID'(`Sym1'),1);
  id `AuxFuncID'(`Sym1'?,`Sym2'?,?a) = `FuncID'(`Sym1',`Sym2',?a);
  Argument `SFuncID';
    FactArg `AuxFuncID';
    id `AuxFuncID'(?a,`SqrtFuncID'(`Sym1'?),?b) = `SqrtFuncID'(`Sym1')*`AuxFuncID'(?a,?b);
    Transform,`AuxFuncID',MulArgs(1,last);
    id `AuxFuncID' = `AuxFuncID'(1);
  EndArgument;
  id `SFuncID'(`SqrtFuncID'(`Sym1'?)*`AuxFuncID'(`Sym2'?),?a) = 
    `SFuncID'(`SqrtFuncID'(`Sym1'),`Sym2',?a)
  ;
#endprocedure
#procedure HypConvertToExtraSymbolForm(?variables)
  #If ("`?variables'" == "")
    #Write "Only works with a non-empty list of variables..."
    #Write "HypConvertToExtraSymbolForm"
    #Terminate
  #EndIf
  If (count(HYPLinfRegInfZero,1,HYPLx,1,HYPL,1) > 0);
    Print "Conversion only works on expressions free of hyperlogs...";
    Print "It is better to start the full integration in the package";
    Print "Problematic term: %t";
    Exit;
  EndIf;
  #Define HYPtempOldExtras "0"
  .sort:Start extrasymbol conv;
  PolyRatFun;
  .sort:PolyRatFun is off;
  #Define HYPoldExtras "0"
  #Define HYPnewExtras "0"
  #call HypPolyArgsToExtraSymbols(HYPnum,HYPfact,HYPwrap,1,1,HYPoldExtras,HYPnewExtras,HYPn1,HYPn2,)
  #write "For numerators `HYPnewExtras' extra symbols were created"
  #call HypPolyArgsToExtraSymbols(HYPden,HYPfact,HYPwrap,1,1,HYPoldExtras,HYPnewExtras,HYPn1,HYPn2,)
  #write "For denominators `HYPnewExtras' extra symbols were created"
  #call HypPolyArgsToExtraSymbols(HYPlog,HYPfact,HYPwrap,1,1,HYPoldExtras,HYPnewExtras,HYPn1,HYPn2,)
  #write "For logs `HYPnewExtras' extra symbols were created"
  #Redefine HYPvarDepVars "`?variables'"
  #If (`ExtraSymbols_' > 0)
    #call HypFillVarDepTable(TempExprPrefix,HYPtempCount,`HYPtempOldExtras',`ExtraSymbols_',HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn1,HYPn2)
  #EndIf
  .sort:Variable dependence is stored;
  PolyRatFun HYPratep;
#endprocedure
#procedure HypCompressCoeffs(EvalFuncID,MarkFuncID,VarDepTblID,VarsList,NumberOfLettersVar,SymbolID,RatFuncID,TempExprPrefix,TempExprCountVar,?ArgCases)
  #Define NumberOfExtrasAtStartup "`ExtraSymbols_'"
  #Define OldExtras "0"
  #Define NewExtras "0"
  #Define TotalNewExtras "0"
  #Do ArgFunc={`?ArgCases',}
    #If ("`ArgFunc'" != "")
      #call HypEvalExtraSymbolArgs(`EvalFuncID',OldExtras,NewExtras,`SymbolID'1,`SymbolID'2,``ArgFunc'')
      #If (`NewExtras' > 0)
        #Redefine TotalNewExtras "{`TotalNewExtras'+`NewExtras'}"
      #EndIf
    #EndIf
  #EndDo
  #If (`TotalNewExtras' > 0)
    #If ("`RatFuncID'" != "")
      .sort:Turning off PolyRatFun;
      PolyRatFun;
      .sort:PolyRatFun is off;
    #EndIf
    #call HypFillVarDepTable(`TempExprPrefix',`TempExprCountVar',`NumberOfExtrasAtStartup',`ExtraSymbols_',`MarkFuncID',`VarDepTblID',`VarsList',`NumberOfLettersVar',`SymbolID'1,`SymbolID'2)
    #If ("`RatFuncID'" != "")
      .sort:Turning on PolyRatFun;
      PolyRatFun `RatFuncID';
    #EndIf
  #EndIf
#endprocedure
#procedure HypWrapArgs(WrapFuncID,FuncID,?ArgCases)
  #call HypStartArgCases(`?ArgCases')
  Transform,`FuncID',Replace(1,last)=(xarg_,`WrapFuncID'(xarg_));
  #call HypFinishArgCases(`?ArgCases')
#endprocedure
#procedure HypVarDepExtraSymbolsToExpression(Var,FirstExtra,LastExtra,VarDepTableID,VarListVar,TempExprPrefix,TempExprCountVar,VarDepExprIdVar,DropHigherDependence)
  #Define HYPtempNumVars ""
  #call HypCountListItems(HYPtempNumVars,``VarListVar'')
  #Define HYPtempVarPosition ""
  #call HypFindItemLocationInList(Var,HYPtempVarPosition,``VarListVar'')
  #If (`HYPtempVarPosition' < 1)
    #Write "`Var' is not among `VarListVar'..."
    #Write "`VarListVar' : >>>``VarListVar''<<<"
    #Write "HypVarDepExtraSymbolsToExpression"
    #Terminate
  #EndIf
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  #Redefine `VarDepExprIdVar' "`TempExprPrefix'``TempExprCountVar''"
  local ``VarDepExprIdVar'' =
    #Do iExtra={`FirstExtra'+1},`LastExtra'
      #$HYPtempVarDep = `VarDepTableID'(`iExtra');
      #$HYPtempVarDep = `$HYPtempVarDep'*theta_(`$HYPtempVarDep' - 2^(`HYPtempNumVars'-`HYPtempVarPosition'));
      #If (`DropHigherDependence' == 1)
        #Inside $HYPtempVarDep;
          Multiply thetap_(2^(`HYPtempNumVars'-`HYPtempVarPosition'+1) - `$HYPtempVarDep');
        #EndInside
      #Else
        #Do iPos=1,{`HYPtempVarPosition'-1}
          #$HYPtempVarDep = mod_(`$HYPtempVarDep',2^(`HYPtempNumVars'-`iPos'));
        #EndDo
      #EndIf
      #$HYPtempVarDep = theta_(`$HYPtempVarDep' - 2^(`HYPtempNumVars'-`HYPtempVarPosition'));
      #If (`$HYPtempVarDep' > 0)
        + Z`iExtra'_
      #EndIf
    #EndDo
  ;
#endprocedure
#procedure HypExtraSymbolVarInverseToArgs(ExtraSymbolExprID,Var,InverseFuncID,PolyFuncID,EvalFuncID,MarkFuncID,SymbolID,VarDepTblID,VarListVar,NumberOfLettersVar,TempExprPrefix,TempExprCountVar)
  #If ("`ActiveExprNames_'" != "`ExtraSymbolExprID'")
    #Write "This routine should only be called with `ExtraSymbolExprID' being active..."
    #Write "Current actives: `ActiveExprNames_'"
    #Write "HypExtraSymbolVarInverseToArgs"
    #Terminate
  #EndIf
  If (match(`InverseFuncID'(?a)) > 0);
    Print "`InverseFuncID' already present in term...";
    Print "Routine should only be called on a bare sum of extra symbols...";
    Print "TERM: %t";
    Print "HypExtraSymbolVarInverseToArgs";
    Exit;
  EndIf;
  #If (TermsIn(`ExtraSymbolExprID') > 0)
    id `SymbolID'1?symbol_ = `SymbolID'1*`InverseFuncID'(`SymbolID'1);
    Argument `InverseFuncID';
      FromPolynomial;
    EndArgument;
    #call HypGetPolyCoefficients(`InverseFuncID',`Var',`SymbolID'1)
    Transform,`InverseFuncID',replace(1,last)=(xarg_,`EvalFuncID'(xarg_));
    #Define InverseCase "`InverseFuncID'"
    #call HypCompressCoeffs(`EvalFuncID',`MarkFuncID',`VarDepTblID',`VarListVar',`NumberOfLettersVar',`SymbolID',,`TempExprPrefix',`TempExprCountVar',InverseCase)
    id `InverseFuncID'(?a) = `InverseFuncID'(`PolyFuncID'(?a));
    Argument `InverseFuncID';
      #call HypApplyOneOverZToNum(`PolyFuncID',`Var',`SymbolID')
    EndArgument;
    .sort
  #EndIf
#endprocedure
#procedure HypExtraSymbolZeroLimitsToArgs(ExtraSymbolExprID,Var,LimitFuncID,EvalFuncID,MarkFuncID,SymbolID,VarDepTblID,VarListVar,NumberOfLettersVar,TempExprPrefix,TempExprCountVar)
  #If ("`ActiveExprNames_'" != "`ExtraSymbolExprID'")
    #Write "This routine should only be called with `ExtraSymbolExprID' being active..."
    #Write "Current actives: `ActiveExprNames_'"
    #Write "HypExtraSymbolZeroLimitsToArgs"
    #Terminate
  #EndIf
  If (match(`LimitFuncID'(?a)) > 0);
    Print "`LimitFuncID' already present in term...";
    Print "Routine should only be called on a bare sum of extra symbols...";
    Print "TERM: %t";
    Print "HypExtraSymbolZeroLimitsToArgs";
    Exit;
  EndIf;
  #If (TermsIn(`ExtraSymbolExprID') > 0)
    id `SymbolID'1?symbol_ = `SymbolID'1*`LimitFuncID'(`SymbolID'1);
    Argument `LimitFuncID';
      FromPolynomial;
      Multiply Replace_(`Var',0);
    EndArgument;
    id `LimitFuncID'(`SymbolID'1?) = `LimitFuncID'(`EvalFuncID'(`SymbolID'1));
    #Define LimitCase "`LimitFuncID'"
    #call HypCompressCoeffs(`EvalFuncID',`MarkFuncID',`VarDepTblID',`VarListVar',`NumberOfLettersVar',`SymbolID',,`TempExprPrefix',`TempExprCountVar',LimitCase)
  #EndIf
#endprocedure
#procedure HypTagVarDepFuncsThruExtras(Var,FuncID,TaggedFuncID,WrapFuncID,AuxFuncID,DepExtrasExprID)
  #If ("`DepExtrasExprID'" == "")
    #Write "The expression holding the variable-dependent extras not initialized..."
    #Write "HypTagVarDepFuncsThruExtras"
    #Terminate
  #EndIf
  #If ("`AuxFuncID'" == "`TaggedFuncID'")
    #Write "AuxFuncID and TaggedFuncID need to be genuinely different..."
    #Write "AuxFuncID = `AuxFuncID'"
    #Write "TaggedFuncID = `TaggedFuncID'"
    #Write "HypTagVarDepFuncsThruExtras"
    #Terminate
  #EndIf
  Bracket `FuncID';
  .sort:Bracketed `FuncID';
  Keep Brackets;
  id `FuncID'(?a) = `WrapFuncID'(`AuxFuncID'(?a));
  Argument `WrapFuncID';
    #If ("`Var'" != "")
      If (Occurs(`Var'));
        Multiply Replace_(`AuxFuncID',`TaggedFuncID');
      EndIf;
    #EndIf
    #Do extra=`DepExtrasExprID'
      If (Occurs(`extra'));
        Multiply Replace_(`AuxFuncID',`TaggedFuncID');
      EndIf;
    #EndDo
  EndArgument;
  id `WrapFuncID'(`TaggedFuncID'(?a)) = `TaggedFuncID'(?a);
  id `WrapFuncID'(`AuxFuncID'(?a)) = `FuncID'(?a);
#endprocedure
#procedure HypFactorizeWrappedArgs(WrapFuncID,SymbolID,?ArgCases)
  #call HypStartArgCases(`?ArgCases')
  If (match(`WrapFuncID'(`SymbolID'1?,`SymbolID'2?,?a)) > 0);
    Print "`WrapFuncID' should have a single argument...";
    Print "TERM: %t";
    Print "HypFactorizeWrappedArgs";
    Exit;
  EndIf;
  FactArg `WrapFuncID';
  ChainOut `WrapFuncID';
  id `WrapFuncID'(`SymbolID'1?number_) = `SymbolID'1;
  id `WrapFuncID'(`SymbolID'1?symbol_) = `SymbolID'1;
  id `WrapFuncID'(1/`SymbolID'1?symbol_) = 1/`SymbolID'1;
  #call HypFinishArgCases(`?ArgCases')
#endprocedure
#procedure HypWrappedArgsToExtraSymbolNumbers(WrapFuncID,IndexFuncID,NumberOfOldExtrasVar,NumberOfExtrasVar,SymbolID,?ArgCases)
  #Redefine `NumberOfOldExtrasVar' "`ExtraSymbols_'"
  #call HypStartArgCases(`?ArgCases')
  If (match(`WrapFuncID'(`SymbolID'1?,`SymbolID'2?,?a)) > 0);
    Print "`WrapFuncID' should have a single argument...";
    Print "TERM: %t";
    Print "HypWrappedArgsToExtraSymbolNumbers";
    Exit;
  EndIf;
  ArgToExtrasymbol,ToNumber,`WrapFuncID';
  id `WrapFuncID'(`SymbolID'1?number_) = `IndexFuncID'(`SymbolID'1);
  #call HypFinishArgCases(`?ArgCases')
  .sort:`WrapFuncID' args to number extras;
  #Redefine `NumberOfExtrasVar' "`ExtraSymbols_'"
#endprocedure
#procedure HypExtraSymbolNumbersToExpression(ExtraExprIDvar,IndexFuncID,AuxFuncID,SymbolID,FirstExtra,LastExtra,TempExprPrefix,TempExprCountVar,DropExtras)
  #If ("`ActiveExprNames_'" != "")
    #Write "No active expressions are allowed when this routine is called..."
    #Write "Active expressions: `ActiveExprNames_'"
    #Write "HypExtraSymbolNumbersToExpression"
    #Terminate
  #EndIf
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  #Redefine `ExtraExprIDvar' "`TempExprPrefix'``TempExprCountVar''"
  local ``ExtraExprIDvar'' = 
    #If (`FirstExtra' < `LastExtra')
      <Z{`FirstExtra'+1}_*`IndexFuncID'({`FirstExtra'+1})> 
      + ... + 
      <Z{`LastExtra'}_*`IndexFuncID'({`LastExtra'})>
    #EndIf
  ;
  FromPolynomial;
  .sort:``ExtraExprIDvar'' opened;
  #If (`DropExtras' == 1)
    #If (`ExtraSymbols_' > `LastExtra')
      #Write "LastExtra is not up-to-date, we have more extras..."
      #Write "They will be deleted as well..."
      #Write "LastExtra = `LastExtra'"
      #Write "Current count = `ExtraSymbols_'"
      #Write "HypExtraSymbolNumbersToExpression"
      #Terminate
    #EndIf
    #If (`FirstExtra' < `LastExtra')
      Delete,Extrasymbols>`FirstExtra';
    #EndIf
  #EndIf
  If (match(1/`SymbolID'1?symbol_) > 0);
    Print "Symbol on negative power after first FromPolynomial...";
    Print "TERM: %t";
    Print "HypExtraSymbolNumbersToExpression";
    Exit;
  EndIf;
  #If (`FirstExtra' > 0)
    Bracket <Z1_>,...,<Z`FirstExtra'_>;
    .sort:Bracketed in old extras;
    Keep Brackets;
  #EndIf;
  $HYPp = 1;
  Repeat id `SymbolID'1?symbol_^`SymbolID'2?!{0,} = `AuxFuncID'1(`SymbolID'1)^`SymbolID'2;
  While (count(`AuxFuncID'1,1) > 0);
    id once `AuxFuncID'1(`SymbolID'1?$HYPz) = 1;
    $HYPp = $HYPp*extrasymbol_($HYPz);
  EndWhile;
  Multiply $HYPp;
  id extrasymbol_(`SymbolID'1?) = `SymbolID'1;
  ModuleOption,local,$HYPp,$HYPz;
  .sort:Atoms multiplied;
#endprocedure
#procedure HypExtraSymbolNumbersToExpressionLite(ExtraExprIDvar,IndexFuncID,FirstExtra,LastExtra,TempExprPrefix,TempExprCountVar,DropExtras)
  #If ("`ActiveExprNames_'" != "")
    #Write "No active expressions are allowed when this routine is called..."
    #Write "Active expressions: `ActiveExprNames_'"
    #Write "HypExtraSymbolNumbersToExpressionLite"
    #Terminate
  #EndIf
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  #Redefine `ExtraExprIDvar' "`TempExprPrefix'``TempExprCountVar''"
  local ``ExtraExprIDvar'' = 
    #If (`FirstExtra' < `LastExtra')
      <Z{`FirstExtra'+1}_*`IndexFuncID'({`FirstExtra'+1})> 
      + ... + 
      <Z{`LastExtra'}_*`IndexFuncID'({`LastExtra'})>
    #EndIf
  ;
  FromPolynomial;
  .sort:``ExtraExprIDvar'' opened;
  #If (`DropExtras' == 1)
    #If (`ExtraSymbols_' > `LastExtra')
      #Write "LastExtra is not up-to-date, we have more extras..."
      #Write "They will be deleted as well..."
      #Write "LastExtra = `LastExtra'"
      #Write "Current count = `ExtraSymbols_'"
      #Write "HypExtraSymbolNumbersToExpressionLite"
      #Terminate
    #EndIf
    #If (`FirstExtra' < `LastExtra')
      Delete,Extrasymbols>`FirstExtra';
    #EndIf
  #EndIf
#endprocedure
#procedure HypCollectBracketsToExtraSymbols(ExprID,IndexFuncID,CollectFuncID,OverFlowFuncID,PolyRatFuncID,SymbolID,Percentage)
  #If ("`ActiveExprNames_'" != "`ExprID'")
    #Write "We should only have `ExprID' as active expression..."
    #Write "Actives: `ActiveExprNames_'"
    #Write "HypCollectBracketsToExtraSymbols"
    #Terminate
  #EndIf
  #If ("`PolyRatFuncID'" != "")
    If (count(`PolyRatFuncID',1) > 0);
      Print "It seems PolyRatFun option is active...";
      Print "Found `PolyRatFuncID'";
      Print "TERM: %t";
      Exit;
    EndIf;
  #EndIf
  Bracket `IndexFuncID';
  .sort:Bracketed `IndexFuncID';
  Collect `CollectFuncID',`OverFlowFuncID',`Percentage';
  If (count(`OverFlowFuncID',1) > 0);
    Print "Collect to `CollectFuncID' overflowed: `Percentage' % MaxTermSize";
    Print "HypCollectBracketsToExtraSymbols";
    Exit;
  EndIf;
  .sort:Collected to `CollectFuncID';
  #call HypFactorizeWrappedArgs(`CollectFuncID',`SymbolID',)
  .sort:Args factorized;
  ArgToExtrasymbol,`CollectFuncID';
  id `CollectFuncID'(`SymbolID'1?) = `SymbolID'1;
  .sort:Args to extras done;
#endprocedure
#procedure HypCompactAvatars(Var,RatFuncID)
  #$HYPtempCheck = HYPaux1(`HYPvarDepVars');
  #Inside $HYPtempCheck
    If (match(HYPaux1(?a,`Var',?b)) == 0);
      Print "`Var' is not in >>>`HYPvarDepVars'<<<";
      Print "HypCompactAvatars";
      Exit;
    EndIf;
  #EndInside
  #If (`HYPnumExtraSymbolLetters' != `Extrasymbols_')
    #Write "We have further extra symbols beside the avatars..."
    #Write "The routine will kill them..."
    #Write "Number of avatars: `HYPnumExtraSymbolLetters'"
    #Write "Number of extras:  `Extrasymbols_'"
    #Terminate
  #EndIf
  #Define HYPtempOldAvatars "`Extrasymbols_'"
  #If ("`RatFuncID'" != "")
    PolyRatFun;
  #EndIf
  #call HypIncrementPreprocCounter(HYPtempCount)
  #Define HYPtempContentExpr "`HYPtempPrefix'`HYPtempCount'"
  local `HYPtempContentExpr' = 
    #If (`HYPnumExtraSymbolLetters' > 0)
      <Z1_> + ... + <Z`HYPnumExtraSymbolLetters'_>
    #EndIf
  ;
  .sort:Avatar sum expr created;
  #call HypExpressionToDynamicSetInFile(`HYPtempContentExpr',avatars.expr,avatars.h)
  #call HypSymbolsToNumbersInFuncArgWithSetInFile(avatars.h,HYPtag,HYPn)
  Argument;
    #call HypSymbolsToNumbersInFuncArgWithSetInFile(avatars.h,HYPtag,HYPn)
  EndArgument;
  Argument;
    Argument;
      #If (`HYPnumberOfUserParameters' == 0)
        If (match(HYPn1?symbol_^HYPn2?) > 0);
          Print "No extra is allowed at this level, no conversion will take place...";
          Print "ARG: %t";
          Print "HypCompactAvatars";
          Exit;
        EndIf;
      #Else
        If (match(HYPn1?!{`HYPuserParameters'}^HYPn2?) > 0);
          Print "No extra is allowed at this level, no conversion will take place...";
          Print "ARG: %t";
          Print "HypCompactAvatars";
          Exit;
        EndIf;
      #EndIf
    EndArgument;
  EndArgument;
  .sort:Avatars to functions;
  #call HypHideActiveExpressions(`HYPtempContentExpr')
  .sort
  #$HYPtempBinaryPosition = HYPaux1(`HYPvarDepVars');
  #Inside $HYPtempBinaryPosition;
    id HYPaux1(?a,`Var',?b) = 2^(nargs_(?b));
  #EndInside;
  id HYPtag(HYPn1?) = HYPcoeff(HYPn1)*HYPtag(HYPvarDepTbl(HYPn1));
  id HYPtag(HYPn1?{>=`$HYPtempBinaryPosition'}) = 0;
  Multiply Replace_(HYPtag,HYPvars);
  id HYPcoeff(HYPn1?) = HYPcoeff(extrasymbol_(HYPn1))*HYPtag(HYPn1);
  .sort
  ClearTable HYPvarDepTbl;
  Delete,Extrasymbols;
  #Redefine HYPnumExtraSymbolLetters "0"
  .sort
  ArgToExtrasymbol,HYPcoeff;
  .sort
  #call HypIncrementPreprocCounter(HYPtempCount)
  #Define HYPtempVarsExpr "`HYPtempPrefix'`HYPtempCount'"
  local `HYPtempVarsExpr' = `HYPtempContentExpr';
  If (Expression(`HYPtempVarsExpr') == 1);
    id HYPcoeff(HYPn1?)*HYPtag(HYPn2?) = HYPn1;
    id HYPvars(HYPn1?) = HYPn1;
  ElseIf (Expression(`HYPtempContentExpr') == 1);
    id HYPvars(HYPn1?) = 1;
    id HYPcoeff(HYPn1?) = HYPn1;
  EndIf;
  .sort
  Skip;
  nSkip `HYPtempContentExpr',`HYPtempVarsExpr';
  #call HypIncrementPreprocCounter(HYPtempCount)
  local `HYPtempPrefix'`HYPtempCount' = 
    #If (`Extrasymbols_' > 0)
      <Z1_> + ... + <Z`Extrasymbols_'_>
    #EndIf
  ;
  .sort
  #call HypExpressionToDynamicSetInFile(`HYPtempPrefix'`HYPtempCount',avatars.expr,avatars.h)
  If (Expression(`HYPtempVarsExpr') == 1);
    #call HypSymbolsToNumbersInFuncArgWithSetInFile(avatars.h,HYPtag,HYPn)
  EndIf;
  .sort
  Bracket HYPtag;
  .sort
  FillExpression HYPsubstTbl = `HYPtempContentExpr'(HYPtag);
  FillExpression HYPvarDepTbl = `HYPtempVarsExpr'(HYPtag);
  .sort
  #call HypUnhideActiveExpressions
  Drop `HYPtempContentExpr',`HYPtempVarsExpr',`HYPtempPrefix'`HYPtempCount';
  .sort
  Multiply Replace_(HYPtag,HYPsubstTbl);
  If (occurs(HYPsubstTbl));
    Print "a dropped avatar letter survived: %t";
    Print "HypCompactAvatars";
    Exit;
  EndIf;
  .sort 
  #Redefine HYPnumExtraSymbolLetters "`Extrasymbols_'"
  ClearTable HYPsubstTbl;
  #If ("`RatFuncID'" != "")
    PolyRatFun `RatFuncID';
  #EndIf
  #Message Purged {`HYPtempOldAvatars'-`Extrasymbols_'} avatars
  #Message Currently having `Extrasymbols_' avatar letters
#endprocedure
#procedure HypRollSymbolsToPolyRatFun(PolyRatFuncID,SymbolID,ExceptionSetID)
  If (count(`PolyRatFuncID',1) == 0);
    Print "Term without `PolyRatFuncID', do we have PolyRatFun activated?";
    Print "TERM: %t";
    Print "HypRollSymbolsToPolyRatFun";
    Exit;
  EndIf;
  Repeat;
    id `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`SymbolID'3?!`ExceptionSetID'^`SymbolID'4?{>0} =
      `PolyRatFuncID'(`SymbolID'1*`SymbolID'3^`SymbolID'4,`SymbolID'2)
    ;
    also `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`SymbolID'3?!`ExceptionSetID'^`SymbolID'4?{<0} =
      `PolyRatFuncID'(`SymbolID'1,`SymbolID'2*`SymbolID'3^(-`SymbolID'4))
    ;
  EndRepeat;
#endprocedure
#procedure HypRollNumsDensToPolyRatFun(PolyRatFuncID,NumFuncID,DenFuncID,SymbolID)
  If (count(`PolyRatFuncID',1) == 0);
    Print "Term without `PolyRatFuncID', do we have PolyRatFun activated?";
    Print "TERM: %t";
    Print "HypRollNumsDensToPolyRatFun";
    Exit;
  EndIf;
  Argument denom_;
    If (count(`NumFuncID',1,`DenFuncID',1) > 0);
      Print "Negative powers are not allowed...";
      Print "ARG: %t";
      Print "HypRollNumsDensToPolyRatFun";
      Exit;
    EndIf;
  EndArgument;
  If (match(`DenFuncID'(0)) > 0);
    Print "Division by zero due to `DenFuncID' with an argument of zero...";
    Print "TERM: %t";
    Print "HypRollNumsDensToPolyRatFun";
    Exit;
  EndIf;
  If (match(`NumFuncID'?{`NumFuncID',`DenFuncID'}(`SymbolID'1?,`SymbolID'2?,?a)) > 0);
    Print "Multiple arguments in `NumFuncID' and `DenFuncID' are not allowed...";
    Print "TERM: %t";
    Print "HypRollNumsDensToPolyRatFun";
    Exit;
  EndIf;
  Repeat;
    id `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`NumFuncID'(`SymbolID'3?) =
      `PolyRatFuncID'(`SymbolID'1*`SymbolID'3,`SymbolID'2)
    ;
    also `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`DenFuncID'(`SymbolID'3?) =
      `PolyRatFuncID'(`SymbolID'1,`SymbolID'2*`SymbolID'3)
    ;
  EndRepeat;
#endprocedure
#procedure HypRollOutSymbolCoeffs(PolyRatFuncID,NumFuncID,DenFuncID,CoeffFuncID,EpsSymbolID,SymbolID)
  If (count(`PolyRatFuncID',1) == 0);
    Print "Term without `PolyRatFuncID', do we have PolyRatFun activated?";
    Print "TERM: %t";
    Print "HypRollOutSymbolCoeffs";
    Exit;
  EndIf;
  If ((count(`NumFuncID',1) != 0) || (count(`DenFuncID',1) != 0));
    Print "This routine only works without `NumFuncID' and `DenFuncID' not present...";
    Print "TERM: %t";
    Print "HypRollOutSymbolCoeffs";
    Exit;
  EndIf;
  .sort:Disarming PolyRatFun;
  PolyRatFun;
  id `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?) = 
    `PolyRatFuncID'(1,1)*`NumFuncID'(`SymbolID'1)*`DenFuncID'(`SymbolID'2)
  ;
  FactArg `DenFuncID';
  Repeat;
    id `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`NumFuncID'(`SymbolID'3?number_) =
      `PolyRatFuncID'(`SymbolID'1*`SymbolID'3,`SymbolID'2)
    ;
    also `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`DenFuncID'(?a,`SymbolID'3?number_,?b) =
      `PolyRatFuncID'(`SymbolID'1,`SymbolID'2*`SymbolID'3)*`DenFuncID'(?a,?b)
    ;
    also `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`DenFuncID'(?a,`EpsSymbolID',?b) =
      `PolyRatFuncID'(`SymbolID'1,`SymbolID'2*`EpsSymbolID')*`DenFuncID'(?a,?b)
    ;
    id `DenFuncID' = 1;
  EndRepeat;
  Argument `DenFuncID';
    If (Occurs(`EpsSymbolID') > 0);
      Print "`DenFuncID' contains non-factorizable `EpsSymbolID'...";
      Print "ARG: %t";
      Print "HypRollOutSymbolCoeffs";
      Exit;
    EndIf;
  EndArgument;
  If (Match(`DenFuncID'(?a,`SymbolID'1?!symbol_,?b)) > 0);
    Print "`DenFuncID' does not factorize into symbol factors...";
    Print "TERM: %t";
    Print "HypRollOutSymbolCoeffs";
    Exit;
  EndIf;
  ChainOut `DenFuncID';
  #call HypGetPolyCoefficients(`NumFuncID',`EpsSymbolID',`SymbolID'1)
  Multiply Replace_(`NumFuncID',`CoeffFuncID');
  Repeat;
    id `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`CoeffFuncID'(0,?a) =
      `PolyRatFuncID'(`SymbolID'1*`EpsSymbolID',`SymbolID'2)*`CoeffFuncID'(?a)
    ;
    also `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`CoeffFuncID'(`SymbolID'3?number_,?a) =
      `PolyRatFuncID'(`SymbolID'1*`SymbolID'3,`SymbolID'2)
      +
      `PolyRatFuncID'(`SymbolID'1*`EpsSymbolID',`SymbolID'2)*`CoeffFuncID'(?a)
    ;
    also `PolyRatFuncID'(`SymbolID'1?,`SymbolID'2?)*`CoeffFuncID'(`SymbolID'3?!{0,},?a) =
      `PolyRatFuncID'(`SymbolID'1,`SymbolID'2)*`NumFuncID'(`SymbolID'3)
      +
      `PolyRatFuncID'(`SymbolID'1*`EpsSymbolID',`SymbolID'2)*`CoeffFuncID'(?a)
    ;
    id `CoeffFuncID' = 0;
  EndRepeat;
#endprocedure
#procedure HypExtraSymbolsToExpression(ExprID, FirstExtraSymbol, LastExtraSymbol, ToWrap, WrapFuncID, ToTag, TagFuncID, OnlyIfNewExtras)
  #if ((`OnlyIfNewExtras' == 0) || (`LastExtraSymbol' > `FirstExtraSymbol'))
    .sort:ExtraSymbolsToExpression starts;
    skip;
    local `ExprID' =
      #do itrm={`FirstExtraSymbol'+1}, `LastExtraSymbol'
        + 
        #if (`ToWrap')
          + `WrapFuncID'(`itrm', Z`itrm'_)
        #elseif (`ToTag')
          + `TagFuncID'(`itrm') * Z`itrm'_
        #else
          + Z`itrm'_
        #endif
      #enddo
    ;
    .sort:ExtraSymbolsToExpression fin;
  #endif
#endprocedure
#procedure HypEncodeNumberofArgsInFunctionHead(FuncID,NewFuncID,Sym,MinNumArg,MaxNumArg,?Exprs)
  .sort:EncodeNumberofArgsInFunctionHead start;
  skip;
  nskip `?Exprs';
  #do inarg=`MinNumArg',`MaxNumArg'
    id `FuncID'(<`Sym'1?>,...,<`Sym'`inarg'?>) = `NewFuncID'`inarg'(<`Sym'1>,...,<`Sym'`inarg'>);
  #enddo
  .sort:EncodeNumberofArgsInFunctionHead fin;
#endprocedure
#procedure HypDetermineMinAndMaxFunctionIndices(FuncID,StartIndex,FinishIndex,FoundMinIndex,FoundMaxIndex,?Exprs)
  .sort:DetermineMinAndMaxFunctionIndices start;
  skip;
  nskip `?Exprs';
  #redefine `FoundMinIndex' "99"
  #redefine `FoundMaxIndex' "0"
  #$MinIndex = 99;
  #$MaxIndex = 0;
  #do iindx=`StartIndex', `FinishIndex'
    if (count(`FuncID'`iindx', 1) > 0);
      $MinIndex = min_($MinIndex, `iindx');
      $MaxIndex = max_($MaxIndex, `iindx');
    endif;
  #enddo
  ModuleOption,Minimum,$MinIndex;
  ModuleOption,Maximum,$MaxIndex;
  .sort:Min/Max determined;
  #if (`$MaxIndex' > 0)
    #redefine `FoundMinIndex' "`$MinIndex'";
    #redefine `FoundMaxIndex' "`$MaxIndex'";
  #endif
  .sort:DetermineMinAndMaxFunctionIndices fin;
#endprocedure
#procedure HypExtractFunctionsToExtraSymbols(FuncID,OldExtraSymbolVar,NewExtraSymbolVar,?Exprs)
  .sort:ExtractFunctionsToExtraSymbols start;
  skip;
  nskip `?Exprs';
  #redefine `OldExtraSymbolVar' "`ExtraSymbols_'"
  bracket `FuncID';
  .sort:`?Exprs' bracketed for extras;
  keep brackets;
  skip;
  nskip `?Exprs';
  ToPolynomial,OnlyFunctions,`FuncID';
  .sort:Extras identified;
  skip;
  #redefine `NewExtraSymbolVar' "{`ExtraSymbols_' - ``OldExtraSymbolVar''}"
  .sort:ExtractFunctionsToExtraSymbols fin;
#endprocedure
#procedure HypCreateFunctionExpressionWithExtras(NewExprID,FuncID,OnlyWhenFunctionsFound,?Exprs)
  #define OldExtraSymbolsForExtract ""
  #define NewExtraSymbolsForExtract ""
  #call HypExtractFunctionsToExtraSymbols(`FuncID',OldExtraSymbolsForExtract,NewExtraSymbolsForExtract,`?Exprs')
  skip;
  #call HypExtraSymbolsToExpression(`NewExprID',{`OldExtraSymbolsForExtract'},{`OldExtraSymbolsForExtract'+`NewExtraSymbolsForExtract'},0,,0,,`OnlyWhenFunctionsFound')
  #if (`NewExtraSymbolsForExtract' > 0)
    #do expr={`?Exprs',`NewExprID'}
      skip;
      nskip `expr';
      #if (`HYPdebug')
        #message FromPoly with {`OldExtraSymbolsForExtract'+`NewExtraSymbolsForExtract'} extras;
      #endif
      FromPolynomial;
      .sort:FromPoly for `expr';
    #enddo
  #else
    #if (`HYPDEBUG')
      #message No new extra symbol got created;
    #endif
  #endif
  .sort:Extras are back to functs in `?Exprs',`NewExprID';
  skip;
  #if (`OldExtraSymbolsForExtract' > 0)
    delete extrasymbols>`OldExtraSymbolsForExtract';
  #else
    delete extrasymbols;
  #endif
  .sort:CreateFunctionExpressionWithExtras fin;
#endprocedure
#procedure HypIsolateAndDissectVarDepFunctionsThruExtra(ExprID,FuncID,AuxFuncID,RatFuncID,PolyFuncID,Var,Sym,MinFuncArg,MaxFuncArg,MinArgLength,MaxArgLength,?Exprs)
  .sort:IsolateAndDissectVarDepFunctionsThruExtra start;
  skip;
  nskip `?Exprs';
  #call HypEncodeNumberofArgsInFunctionHead(`FuncID',`AuxFuncID',`Sym',`MinFuncArg',`MaxFuncArg',`?Exprs')
  #redefine `MinArgLength' "99"
  #redefine `MaxArgLength' "0"
  #call HypDetermineMinAndMaxFunctionIndices(`AuxFuncID',`MinFuncArg',`MaxFuncArg',`MinArgLength',`MaxArgLength',`?Exprs')
  #if (`HYPdebug')
    #message Minimum and maximum args: ``MinArgLength'',``MaxArgLength'';
  #endif
  #do iArgLength=``MinArgLength'',``MaxArgLength''
    #call HypCreateFunctionExpressionWithExtras(`ExprID'`iArgLength',`AuxFuncID'`iArgLength',0,`?Exprs')
    #call HypArgsToRatFunc(`ExprID'`iArgLength', `AuxFuncID'`iArgLength', `RatFuncID', `AuxFuncID', `Sym'1)
    #call HypDropIndepTerms(`ExprID'`iArgLength', `AuxFuncID'`iArgLength', `RatFuncID', `Var', HYPSol, 1, `PolyFuncID', `Sym'1, `Sym'2)
  #enddo
  .sort:`FuncID' isolated;
  #define MinDepArgLength "99"
  #define MaxDepArgLength "0"
  #call HypDetermineMinAndMaxFunctionIndices(`AuxFuncID',``MinArgLength'',``MaxArgLength'',MinDepArgLength,MaxDepArgLength,<`ExprID'``MinArgLength''>,...,<`ExprID'``MaxArgLength''>)
  #if (`HYPdebug')
    #message Minimum and maximum dependent args: `MinDepArgLength',`MaxDepArgLength';
  #endif
  #if (`MinDepArgLength' > ``MinArgLength'')
    drop <`ExprID'``MinArgLength''>,...,<`ExprID'{`MinDepArgLength'-1}>;
  #endif
  #if (`MaxDepArgLength' < ``MaxArgLength'')
    drop <`ExprID'{`MaxDepArgLength'+1}>,...,<`ExprID'``MaxArgLength''>;
  #endif
  .sort:Non-deps dropped;
  #redefine `MaxArgLength' "`MaxDepArgLength'"
  #if (``MaxArgLength'' == 0)
    #redefine `MinArgLength' "99"
  #else
    #redefine `MinArgLength' "`MinDepArgLength'"
  #endif
  skip;
  nskip <`ExprID'``MinArgLength''>,...,<`ExprID'``MaxArgLength''>;
  dropcoefficient;
  .sort:Num coeffs dropped;
  skip;
  nskip `?Exprs',<`ExprID'``MinArgLength''>,...,<`ExprID'``MaxArgLength''>;
  #do iArgLength=`MinFuncArg',`MaxFuncArg'
    multiply replace_(`AuxFuncID'`iArgLength',`FuncID');
  #enddo
  .sort:Isolated and dissected;
#endprocedure
#procedure HypSubstituteToExprsWithExtrasAndTable(SolutionExprID,LHSfuncID,RHSfuncID,AuxFuncID,SolutionTableID,?Exprs)
  .sort:SubstituteToExprsWithExtrasAndTable start;
  #define OldExtrasSubstituteToExprsWithExtrasAndTable "`ExtraSymbols_'"
  skip;
  nskip `SolutionExprID';
  multiply replace_(`RHSfuncID',`AuxFuncID');
  ToPolynomial,OnlyFunctions,`AuxFuncID';
  .sort:`AuxFuncID' to extras `SolutionExprID';
  skip;
  nskip `SolutionExprID';
  #define NewExtrasSubstituteToExprsWithExtrasAndTable "`ExtraSymbols_'"
  #do iextra={`OldExtrasSubstituteToExprsWithExtrasAndTable'+1},`ExtraSymbols_'
    id Z`iextra'_ = `AuxFuncID'(`iextra');
  #enddo
  .sort:Extras to `AuxFuncID';
  skip;
  nskip `SolutionExprID';
  bracket `AuxFuncID';
  .sort:`SolutionExprID' bracketed;
  FillExpression `SolutionTableID' = `SolutionExprID'(`AuxFuncID');
  #do iextra={`OldExtrasSubstituteToExprsWithExtrasAndTable'+1},`ExtraSymbols_'
    id `AuxFuncID'(`iextra') = ExtraSymbol_(`iextra');
  #enddo
  multiply replace_(`AuxFuncID',`RHSfuncID');
  .sort:Sols put into `SolutionTableID';
  skip;
  nskip `?Exprs';
  multiply replace_(`LHSfuncID',`AuxFuncID');
  ToPolynomial,OnlyFunctions,`AuxFuncID';
  .sort:`AuxFuncID' to extras in `?Exprs';
  skip;
  nskip `?Exprs';
  #do iextra={`NewExtrasSubstituteToExprsWithExtrasAndTable'+1},`ExtraSymbols_'
    id Z`iextra'_ = ExtraSymbol_(`iextra');
  #enddo
  multiply replace_(`AuxFuncID',`LHSfuncID');
  .sort
  skip;
  nskip `?Exprs';
  #do iextra={`OldExtrasSubstituteToExprsWithExtrasAndTable'+1},`NewExtrasSubstituteToExprsWithExtrasAndTable'
    id Z`iextra'_ = `AuxFuncID'(`iextra');
  #enddo
  .sort:Extras to `AuxFuncID';
  #if (`OldExtrasSubstituteToExprsWithExtrasAndTable' > 0)
    delete ExtraSymbols>`OldExtrasSubstituteToExprsWithExtrasAndTable';
  #else
    delete ExtraSymbols;
  #endif
  .sort:Extrasymbols purged;
  skip;
  nskip `?Exprs';
  multiply replace_(`AuxFuncID',`SolutionTableID');
  .sort:Substed `?Exprs';
  ClearTable `SolutionTableID';
  skip;
  nskip `SolutionExprID';
  multiply replace_(`AuxFuncID',`RHSfuncID');
  .sort:Cleared table;
  #if 0
  #endif
  .sort:SubstituteToExprsWithExtrasAndTable fin;
#endprocedure
#procedure HypNormalizeNums(NumFuncID,DenFuncID,SymbolID)
  FactArg `NumFuncID';
  ChainOut `NumFuncID';
  Repeat;
    id `NumFuncID'(`SymbolID'1?number_) = `SymbolID'1;
    id `NumFuncID'(1/`SymbolID'1?symbol_) = `DenFuncID'(`SymbolID'1);
  EndRepeat;
#endprocedure
#procedure HypNormalizeDens(DenFuncID,NumFuncID,SymbolID)
  FactArg `DenFuncID';
  ChainOut `DenFuncID';
  Repeat;
    id `DenFuncID'(`SymbolID'1?number_) = 1/`SymbolID'1;
    id `DenFuncID'(1/`SymbolID'1?symbol_) = `NumFuncID'(`SymbolID'1);
  EndRepeat;
#endprocedure
#procedure HypNormalizeNumsDens(NumFuncID,DenFuncID,SymbolID)
  #call HypNormalizeNums(`NumFuncID',`DenFuncID',`SymbolID')
  #call HypNormalizeDens(`DenFuncID',`NumFuncID',`SymbolID')
  Repeat;
    id `NumFuncID'(`SymbolID'1?symbol_)*`DenFuncID'(`SymbolID'1?symbol_) = 1;
  EndRepeat;
  If (match(`NumFuncID'(`SymbolID'1?)*`DenFuncID'(`SymbolID'1?)) > 0);
    Print "Towers not cancelled...";
    Print "Probably multiterm arguments present, this is alien in extrasymbol mode...";
    Print "Affected term: %t";
    Print "HypNormalizeNumsDens";
    Exit;
  EndIf;
#endprocedure
#procedure HypTrivialZeroLimitNumsDens(NumFuncID,DenFuncID,NumCoeffFuncID,DenCoeffFuncID,SymbolID)
  id `NumFuncID'(`SymbolID'?!{0,},?a) = `NumCoeffFuncID'(`SymbolID');
  id `DenFuncID'(`SymbolID'?!{0,},?a) = `DenCoeffFuncID'(`SymbolID');
#endprocedure
#procedure HypNormalizeVarDepNumsDens(VarDepNumFuncID,VarDepDenFuncID,NumCoeffFuncID,DenCoeffFuncID,SymbolID)
  id `VarDepNumFuncID'(`SymbolID'1?) = `NumCoeffFuncID'(`SymbolID'1);
  id `VarDepDenFuncID'(`SymbolID'1?) = `DenCoeffFuncID'(`SymbolID'1);
  #call HypNormalizeNumsDens(`NumCoeffFuncID',`DenCoeffFuncID',`SymbolID')
#endprocedure
#procedure HypNormalizeDensToMonicForm(VarDepDenFuncID,DenFuncID,NumFuncID,AuxFuncID,SymbolID)
  If (match(`VarDepDenFuncID'(?a,0)) > 0);
    Print "This routine is only applicable if the leading coeff in `VarDepDenFuncID' is non-zero...";
    Print "TERM: %t";
    Print "HypNormalizeDensToMonicForm";
    Exit;
  EndIf;
  If (count(`AuxFuncID'1,1) > 0);
    Print "`AuxFuncID'1 is not allowed in expression when this routine is called...";
    Print "TERM: %t";
    Print "HypNormalizeDensToMonicForm";
    Exit;
  EndIf;
  Repeat;
    id,once `VarDepDenFuncID'(`SymbolID'1?,?a,`SymbolID'2?!{1,}$HypTempMonicNorm) = 
      `AuxFuncID'1(`SymbolID'1,?a,`SymbolID'2)
    ;
    If (count(`AuxFuncID'1,1) > 0);
      ModuleOption,Local,$HypTempMonicNorm;
      Argument `AuxFuncID'1;
        Multiply 1/$HypTempMonicNorm;
      EndArgument;
      id `AuxFuncID'1(?a) = `DenFuncID'($HypTempMonicNorm)*`VarDepDenFuncID'(?a);
      #call HypNormalizeDens(`DenFuncID',`NumFuncID',`SymbolID')
    EndIf;
  EndRepeat;
#endprocedure
#procedure HypSymbolsToNumsDens(NumFuncID,DenFuncID,SymbolID,ExceptionSetID)
  If (match(exp_(`SymbolID'1?!`ExceptionSetID',`SymbolID'2?)) > 0);
    Print "Symbol not part of `ExceptionSetID' with non-integer power...";
    Print "This routine is meant to work with numeric exponents only...";
    Print "TERM: %t";
    Print "HypSymbolsToNumsDens";
    Exit;
  EndIf;
  Repeat;
    id `SymbolID'1?!`ExceptionSetID'^`SymbolID'2?{>0} = 
      `NumFuncID'(`SymbolID'1)^`SymbolID'2;
    id `SymbolID'1?!`ExceptionSetID'^`SymbolID'2?{<0} = 
      `DenFuncID'(`SymbolID'1)^(-`SymbolID'2);
  EndRepeat;
#endprocedure
#procedure HypNormalizeLogs(LogFuncID,AuxFuncID,SymbolID)
  FactArg `LogFuncID';
  If (match(`LogFuncID'(?a,`SymbolID'1?{<0,},?b)) > 0);
    Print "`LogFuncID' with a negative factor...";
    Print "At the moment positive definite arguments are treated properly";
    Print "Term: %t";
    Exit;
  EndIf;
  #Do iter=1,1
    If (match(`LogFuncID'(`SymbolID'1?,`SymbolID'1?,?a)) > 0);
      id `LogFuncID'(`SymbolID'1?,`SymbolID'1?,?a) = 
        `LogFuncID'(`AuxFuncID'(`SymbolID'1,`SymbolID'1),?a);
      Repeat id `LogFuncID'(`AuxFuncID'(?a,`SymbolID'1?),`SymbolID'1?,?b) = 
        `LogFuncID'(`AuxFuncID'(?a,`SymbolID'1,`SymbolID'1),?b);
      id `LogFuncID'(`AuxFuncID'(`SymbolID'1?,?a)) = (nargs_(?a)+1)*`LogFuncID'(`SymbolID'1);
      also `LogFuncID'(`AuxFuncID'(`SymbolID'1?,?a),`SymbolID'2?,?b) = 
        (nargs_(?a)+1)*`LogFuncID'(`SymbolID'1) + `LogFuncID'(`SymbolID'2,?b);
    EndIf;
    id,once `LogFuncID'(`SymbolID'1?!{`SymbolID'2?},`SymbolID'2?!{`SymbolID'1?},?a) = 
      `LogFuncID'(`SymbolID'1) + `LogFuncID'(`SymbolID'2,?a);
    Repeat;
      id `LogFuncID'(1/`SymbolID'1?symbol_) = - `LogFuncID'(`SymbolID'1);
      id `LogFuncID'(1) = 0;
    EndRepeat;
    If (match(`LogFuncID'(`SymbolID'1?,`SymbolID'2?,?a)) > 0) Redefine iter "0";
    .sort:NormalizeLogs loop;
  #EndDo
#endprocedure
#procedure HypTrivialZeroLimitLogs(VarDepLogFuncID,LogFuncID,SymbolID)
  id `VarDepLogFuncID'(`SymbolID'?!{0,},?a) = `LogFuncID'(`SymbolID');
#endprocedure
#procedure HypVarLogsToHyperlogs(VarDepLogFuncID,LogFuncID,LfuncID,SymbolID)
  If (match(`VarDepLogFuncID'(`SymbolID'1?,`SymbolID'2?,`SymbolID'3?,?a)) > 0);
    Print "At the moment only linear logs could be turned to hyperlogs...";
    Print "Try to factorize the argument or rationalize...";
    Print "TERM: %t";
    Print "HypVarLogsToHyperlogs";
    Exit;
  EndIf;
  id `VarDepLogFuncID'(`SymbolID'1?!{0,1},`SymbolID'2?) = 
    `LogFuncID'(`SymbolID'1) + `LfuncID'(-`SymbolID'1/`SymbolID'2)
  ;
  also `VarDepLogFuncID'(1,`SymbolID'1?) = 
    `LfuncID'(-1/`SymbolID'1)
  ;
  also `VarDepLogFuncID'(0,`SymbolID'1?!{1,}) = 
    `LogFuncID'(`SymbolID'1) + `LfuncID'(0)
  ;
  also `VarDepLogFuncID'(0,1) = 
    `LfuncID'(0)
  ;
#endprocedure
#procedure HypGetPolyCoeffs(FuncID,Sym,Var,ChopLastZero)
  repeat;
    id `FuncID'(?a,`Sym'?!{0,}) = `FuncID'(?a,rem_(`Sym',`Var'),div_(`Sym',`Var'));
  endrepeat;
  #if (`ChopLastZero')
    id `FuncID'(?a, 0) = `FuncID'(?a);
  #endif
#endprocedure
#procedure HypGetPolyCoefficients(PolyFuncID,Var,Sym1)
  Repeat;
    id `PolyFuncID'(?a,`Sym1'?!{0,}) = `PolyFuncID'(?a,rem_(`Sym1',`Var'),div_(`Sym1',`Var'));
  EndRepeat;
  id `PolyFuncID'(?a,0) = `PolyFuncID'(?a);
#endprocedure
#procedure HypSetPolyFromCoeffs(PolyFuncID,AuxFuncID,Var,Sym1,Sym2,?ArgFunctions)
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      Argument `Arg';
    #EndIf
  #EndDo
  Repeat;
    If (match(`PolyFuncID'(`Sym1'?,`Sym2'?,?a)) > 0);
      $pow = 0;
      ModuleOption,Local,$pow;
      id,once `PolyFuncID'(`Sym1'?,`Sym2'?,?a) = `AuxFuncID'(`Sym1',`Sym2',?a);
      Repeat;
        $pow = $pow + 1;
        id `AuxFuncID'(`Sym1'?,`Sym2'?,?a) = `AuxFuncID'(`Sym1'+`Sym2'*`Var'^$pow,?a);
        id `AuxFuncID'(`Sym1'?) = `PolyFuncID'(`Sym1');
      EndRepeat;
    EndIf;
  EndRepeat;
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      EndArgument;
    #EndIf
  #EndDo
#endprocedure
#procedure HypWrapInversePoly(PolyFuncID,InvPolyFuncID,DenFuncID,?ArgFunctions)
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      Argument `Arg';
    #EndIf
  #EndDo
  Repeat;
    $pow = 0;
    ModuleOption,Local,$pow;
    Argument denom_,`DenFuncID';
      $pow = count_(`PolyFuncID',1);
    EndArgument;
    If ($pow > 0);
      id `DenFuncID'?{`DenFuncID',denom_}(`PolyFuncID'(?a)^$pow) = `InvPolyFuncID'(?a)^$pow;
    EndIf;
  EndRepeat;
  #Do Arg={`?ArgFunctions',}
    #If ("`Arg'" != "")
      EndArgument;
    #EndIf
  #EndDo
#endprocedure
#procedure HypRevealVarAsCoeffs(Var)
  #Define HYPvarPos "0"
  #call HypFindItemLocationInList(Var,HYPvarPos,`HYPvarDepVars')
  #If (`HYPvarPos' == 0)
    #Write "`Var' is not among the encoded variables..."
    #Write "Encoded variables: `HYPvarDepVars'"
    #Write "HypRevealVarAsCoeffs"
    #Terminate
  #EndIf
  #call HypNormalizeNumsDens(HYPnum,HYPden,HYPn)
  #call HypNormalizeLogs(HYPlog,HYPwrap,HYPn)
  .sort:Arguments made atomic;
  PolyRatFun;
  .sort:PolyRatFun turned off;
  #Define HYPtempOldExtras "`ExtraSymbols_'"
  #Define ArgCases "Argnum,Argden,Arglog"
  #Define NumCase "HYPnum"
  #Define DenCase "HYPden"
  #Define LogCase "HYPlog"
  #call HypRestoreExtraSymbolsAsCoeffsByVar(`Var',HYPpoly,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,TMP,HYPtempCount,HYPfact,HYPwrap,HYPaux,HYPmark,HYPtag,HYPvars,HYPsubstTbl,HYPn1,HYPn2,HYPn3,NumCase,DenCase,LogCase)
  #If ({`ExtraSymbols_'-`HYPtempOldExtras'} > 0)
    #call HypFillVarDepTable(TempExprPrefix,HYPtempCount,`HYPtempOldExtras',`ExtraSymbols_',HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn1,HYPn2)
  #EndIf
  Repeat;
    id HYPnum(HYPpoly(HYPn1?)) = HYPnum(HYPn1);
    id HYPden(HYPpoly(HYPn1?)) = HYPden(HYPn1);
    id HYPlog(HYPpoly(HYPn1?)) = HYPlog(HYPn1);
  EndRepeat;
  Repeat;
    id HYPnum(HYPpoly(?a)) = HYPnumz(?a);
    id HYPden(HYPpoly(?a)) = HYPdenz(?a);
    id HYPlog(HYPpoly(?a)) = HYPlogz(?a);
    id HYPnum(`Var') = HYPnumz(0,1);
    id HYPden(`Var') = HYPdenz(0,1);
    id HYPlog(`Var') = HYPlogz(0,1);
  EndRepeat;
  .sort:Reveal over;
  PolyRatFun HYPratep;
#endprocedure
#procedure HypFlattenRationalFunction(RatFuncID,Sym1,Sym2)
  id `RatFuncID'(`Sym1'?,`Sym2'?) = `Sym1'/`Sym2';
#endprocedure
#procedure HypDeconcatenateCoproduct(FuncID,Var,LeftFuncID,LeftVar,RightFuncID,RightVar,AuxFuncID,SymbolID)
  Bracket `FuncID';
  .sort
  Keep brackets;
  Repeat;
    id,once `FuncID'(?a
    #If ("`Var'" != "")
      ,`Var'
    #EndIf
      ) = `AuxFuncID'1(?a)*`AuxFuncID'2;
    Repeat;
      id `AuxFuncID'1(?a,`SymbolID'1?)*`AuxFuncID'2(?b) = `AuxFuncID'1(?a)*`AuxFuncID'2(`SymbolID'1,?b) + `AuxFuncID'3(?a,`SymbolID'1)*`AuxFuncID'4(?b);
      id `AuxFuncID'1()*`AuxFuncID'2(?a) = `AuxFuncID'3*`AuxFuncID'4(?a);
    EndRepeat;
  EndRepeat;
  id `AuxFuncID'3(?a) = `LeftFuncID'(?a
    #If ("`LeftVar'" != "")
      ,`LeftVar'
    #EndIf
  );
  id `AuxFuncID'4(?a) = `RightFuncID'(?a
    #If ("`RightVar'" != "")
      ,`RightVar'
    #EndIf
  );
#endprocedure
#procedure HypExpandHyperlogs(Var,LfuncID,DenFuncID,CoeffFuncID,SymbolID)
  If (match(`LfuncID'(?a,0)) > 0);
    Print "To use expansion last letter of `LfuncID' cannot be zero...";
    Print "TERM: %t";
    Print "HypExpandHyperlogs";
    Exit;
  EndIf;
  Repeat;
    If ((count(`LfuncID',1) > 0) && (count(`Var',1) < 0));
      id,once `LfuncID'(?a) = `CoeffFuncID'(0,?a);
      Repeat;
        $pow = -count_(`Var',1);
        ModuleOption,local,$pow;
        If (count(`CoeffFuncID',1) > 0);
          id `CoeffFuncID'(`SymbolID'1?,?a,`SymbolID'2?!{0,}) = 
            -`DenFuncID'(`SymbolID'2)*
             sum_(
               `SymbolID'3,
               0,
               $pow-`SymbolID'1-1,
               `DenFuncID'(`SymbolID'2)^`SymbolID'3/(`SymbolID'1+`SymbolID'3+1)*`CoeffFuncID'(`SymbolID'1+`SymbolID'3+1,?a)
             )
          ;
          also `CoeffFuncID'(`SymbolID'1?,?a,0) = 1/`SymbolID'1*`CoeffFuncID'(`SymbolID'1,?a);
          id `CoeffFuncID'(`SymbolID'1?) = `Var'^`SymbolID'1;
        EndIf;
      EndRepeat;
    Else;
      id `LfuncID'(?a) = 0;
    EndIf;
  EndRepeat;
#endprocedure
#procedure HypShuffleHyperlogToNonZeroEnd(LfuncID,MarkFuncID,AuxFuncID,SymbolID)
  id `LfuncID'(?a,`SymbolID'1?!{0,},?b,0) = `LfuncID'(?a,`MarkFuncID'(`SymbolID'1),?b,0);
  Repeat;
    id `LfuncID'(?a,`MarkFuncID'(`SymbolID'1?),?b,`SymbolID'2?!{0,},?c,0) = 
      `LfuncID'(?a,`SymbolID'1,?b,`MarkFuncID'(`SymbolID'2),?c,0)
    ;
  EndRepeat;
  #call HypShuffleMarkedArgToRight(`LfuncID',`LfuncID',`LfuncID',`MarkFuncID',`AuxFuncID',`SymbolID')
  id `LfuncID'(?a,`MarkFuncID'(`SymbolID'1?)) = `LfuncID'(?a,`SymbolID'1);
#endprocedure
#procedure HypCreateVarSpectators(UpperBoundSpecName,UpperBoundFileName,LowerBoundSpecName,LowerBoundFileName,DoneSpecName,DoneFileName)
  #If (("`UpperBoundSpecName'" == "`LowerBoundSpecName'") || ("`UpperBoundSpecName'" == "`DoneSpecName'") || ("`LowerBoundSpecName'" == "`DoneSpecName'"))
    #Write "Spectator names need to be distinct..."
    #Write "UpperBoundSpecName: `UpperBoundSpecName'"
    #Write "LowerBoundSpecName: `LowerBoundSpecName'"
    #Write "DoneSpecName: `DoneSpecName'"
    #Write "HypCreateVarSpectators"
    #Terminate
  #EndIf
  #If (("`UpperBoundFileName'" == "`LowerBoundFileName'") || ("`UpperBoundFileName'" == "`DoneFileName'") || ("`LowerBoundFileName'" == "`DoneFileName'"))
    #Write "Spectator filenames need to be distinct..."
    #Write "UpperBoundFileName: `UpperBoundFileName'"
    #Write "LowerBoundFileName: `LowerBoundFileName'"
    #Write "DoneFileName: `DoneFileName'"
    #Write "HypCreateVarSpectators"
    #Terminate
  #EndIf
  CreateSpectator `UpperBoundSpecName',"`UpperBoundFileName'";
  CreateSpectator `LowerBoundSpecName',"`LowerBoundFileName'";
  CreateSpectator `DoneSpecName',"`DoneFileName'";
#endprocedure
#procedure HypRemoveVarSpectators(UpperBoundSpecName,LowerBoundSpecName,DoneSpecName,TempExprPrefix,TempExprCountVar)
  #Do Cont={UpperBoundSpecName,LowerBoundSpecName,DoneSpecName}
    #call HypIncrementPreprocCounter(`TempExprCountVar')
    CopySpectator `TempExprPrefix'``TempExprCountVar'' = ``Cont'';
    .sort:``Cont'' copied for check;
    #If (TermsIn(`TempExprPrefix'``TempExprCountVar'') > 0)
      #Write "Spectator ``Cont'' is not empty..."
      #Write "HypRemoveVarSpectators"
      #Terminate
    #Else
      Drop `TempExprPrefix'``TempExprCountVar'';
      RemoveSpectator ``Cont'';
      .sort:``Cont'' dropped;
    #EndIf
  #EndDo
#endprocedure
#procedure HypStashLimitTerms(LimitFuncID,NumFuncID,DenFuncID,UpperLimitSymbol,LowerLimitSymbol,SymbolID,UpperSpecName,LowerSpecName)
  If (count(`LimitFuncID',1) > 1);
    Print "At most one limit function is allowed per term...";
    Print "Term: %t";
    Print "HypStashLimitTerms";
    Exit;
  ElseIf ((count(`LimitFuncID',1) == 1) && (match(`LimitFuncID'(`SymbolID'1?!{`UpperLimitSymbol',`LowerLimitSymbol'})) > 0));
    Print "Alien limit found in term...";
    Print "Allowed limits: Upper: `UpperLimitSymbol', Lower: `LowerLimitSymbol'";
    Print "Term: %t";
    Print "HypStashLimitTerms";
    Exit;
  EndIf;
  If (match(`LimitFuncID'(`UpperLimitSymbol')) > 0);
    id `LimitFuncID'(`UpperLimitSymbol') = 1;
    #call HypNormalizeNumsDens(`NumFuncID',`DenFuncID',`SymbolID')
    ToSpectator `UpperSpecName';
  ElseIf (match(`LimitFuncID'(`LowerLimitSymbol')) > 0);
    id `LimitFuncID'(`LowerLimitSymbol') = 1;
    #call HypNormalizeNumsDens(`NumFuncID',`DenFuncID',`SymbolID')
    ToSpectator `LowerSpecName';
  EndIf;
#endprocedure
#procedure HypSyncNumDenOrder(RatFuncID,NumFuncID,DenFuncID,AuxFuncID,Sym1,Sym2)
  argument `RatFuncID';
    id `AuxFuncID'?{`NumFuncID',`DenFuncID'}(?a) = `AuxFuncID'(nargs_(?a),?a);
  endargument;
  id `RatFuncID'(`NumFuncID'(`Sym1'?,?a),`DenFuncID'(`Sym2'?,?b)) = 
    `RatFuncID'(`NumFuncID'(`Sym1'-`Sym2',?a),`DenFuncID'(`Sym2'-`Sym1',?b));
  argument `RatFuncID';
    repeat;
      id `AuxFuncID'?{`NumFuncID',`DenFuncID'}(`Sym1'?neg_,?a) = `AuxFuncID'(`Sym1'+1,?a,0);
    endrepeat;
    transform,`NumFuncID',dropargs(1,1);
    #if ("`NumFuncID'" != "`DenFuncID'")
      transform,`DenFuncID',dropargs(1,1);
    #endif
  endargument;
#endprocedure
#procedure HypConvertDetailedRatFuncs(FuncID,RatFuncID,PolyFuncID,Var,Sym1,Sym2,Sym3,?Exprs)
  .sort:Cast back detailed ratfuncs;
  skip;
  nskip `?Exprs';
  argument `FuncID';
    id `RatFuncID'(`Sym1'?, `Sym2'?number_) = `Sym1'/`Sym2';
    id `RatFuncID'(`PolyFuncID'(`Sym1'?), `PolyFuncID'(`Sym2'?)) = `RatFuncID'(`Sym1', `Sym2');
    argument `RatFuncID';
      id `PolyFuncID'(?a) = `PolyFuncID'(1, ?a);
      repeat;
        id `PolyFuncID'(`Sym1'?,`Sym2'?,`Sym3'?,?a) = 
          `PolyFuncID'(`Sym1'+1,`Sym2' + `Sym3'*`Var'^`Sym1',?a);
        id `PolyFuncID'(`Sym1'?,`Sym2'?) = `Sym2';
      endrepeat;
    endargument;
    id `RatFuncID'(`Sym1'?, `Sym2'?number_) = `Sym1'/`Sym2';
  endargument;
#endprocedure
#procedure HypDropTrailingZeroRatFuncCoeffs(FuncID,RatFuncID,PolyFuncID,Sym1,...,Sym2)
  Argument `FuncID';
    Repeat;
      id `RatFuncID'(`PolyFuncID'(`Sym1'?,?a,0),`PolyFuncID'(`Sym2'?,?b,0)) =
        `RatFuncID'(`PolyFuncID'(`Sym1',?a),`PolyFuncID'(`Sym2',?b))
      ;
    EndRepeat;
  EndArgument;
#endprocedure
#procedure HypNormalizeToIntegrate(ExprID,DenFuncID,HlogFuncID,RatFuncID,Sym1,...,Sym4)
  .sort:NormalizeToIntegrate start;
  skip;
  nskip `ExprID';
  bracket `HlogFuncID',`DenFuncID';
  .sort:`ExprID' bracketed;
  keep brackets;
  FactArg,(-1),`DenFuncID',2;
  id `DenFuncID'(`Sym1'?,`Sym2'?,`Sym3'?number_) = `DenFuncID'(`Sym1'/`Sym3',`Sym2');
  Argument `HlogFuncID';
    FactArg,(-1),`RatFuncID',2;
    id `RatFuncID'(`Sym1'?,`Sym2'?,`Sym3'?number_) = `RatFuncID'(`Sym1'/`Sym3',`Sym2');
  EndArgument;
#endprocedure
#procedure HypArgRatFunManip(FuncID,Manip)
  #if ((`Manip' == +) || (`Manip' == -))
    id `FuncID'(HYPrat(HYPn1?,HYPn2?),0) = `FuncID'(HYPrat(HYPn1,HYPn2));
    id `FuncID'(0,HYPrat(HYPn1?,HYPn2?)) = `FuncID'(HYPrat((`Manip'1)*HYPn1,HYPn2));
    id `FuncID'(HYPn1?number_,HYPrat(HYPn2?,HYPn3?)) = 
      `FuncID'(HYPrat(HYPn1*HYPn3+(`Manip'1)*HYPn2,HYPn3));
    id `FuncID'(HYPrat(HYPn1?,HYPn2?),HYPn3?number_) = 
      `FuncID'(HYPrat(HYPn1+(`Manip'1)*HYPn3*HYPn2,HYPn2));
    id `FuncID'(HYPrat(HYPn1?,HYPn2?!{HYPn4?}),HYPrat(HYPn1?,HYPn4?!{HYPn2?})) =
      `FuncID'(HYPn1)*`FuncID'(HYPrat(HYPn4 + (`Manip'1)*HYPn2,mul_(HYPn2,HYPn4)));
    id `FuncID'(HYPrat(HYPn1?!{HYPn3?},HYPn2?),HYPrat(HYPn3?!{HYPn1?},HYPn2?)) =
      `FuncID'(HYPrat(HYPn1 + (`Manip'1)*HYPn3,HYPn2));
    id `FuncID'(HYPrat(HYPn1?,HYPn2?),HYPrat(HYPn3?,HYPn4?)) =
      `FuncID'(gcd_(HYPn2,HYPn4),HYPrat(HYPn1,HYPn2),HYPrat(HYPn3,HYPn4));
    id `FuncID'(HYPn5?,HYPrat(HYPn1?,HYPn2?),HYPrat(HYPn3?,HYPn4?)) =
      `FuncID'(div_(HYPn2,HYPn5),div_(HYPn4,HYPn5),HYPrat(HYPn1,HYPn2),HYPrat(HYPn3,HYPn4));
    id `FuncID'(HYPn5?,HYPn6?,HYPrat(HYPn1?,HYPn2?),HYPrat(HYPn3?,HYPn4?)) =
      `FuncID'(HYPrat(HYPn1*HYPn6 + (`Manip'1)*HYPn3*HYPn5,HYPn5*HYPn4));
    id `FuncID'(HYPn1?,HYPrat(HYPn2?,HYPn3?)) =
      `FuncID'(HYPrat(HYPn1*HYPn3 + (`Manip'1)*HYPn2,HYPn3));
    id `FuncID'(HYPrat(HYPn1?,HYPn2?),HYPn3?) =
      `FuncID'(HYPrat(HYPn1 + (`Manip'1)*HYPn3*HYPn2,HYPn2));
  #else
    if (count(`FuncID',1) > 0);
      print "case is not covered yet...";
      print "%t";
      print "ArgRatFunManip";
      exit;
    endif;
  #endif
#endprocedure
#procedure HypCancelTowers(Var)
  If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?,HYPn4?,?a)) > 0);
    Print "At the moment we can only cancel towers with at most quadratic denominators.";
    Print "TERM: %t";
    Print "HypCancelTowers";
    Exit;
  EndIf;
  Repeat;
    id,once `Var'*HYPdenz(0,HYPa1?) = HYPden(HYPa1);
    also,once `Var'*HYPdenz(HYPb0?,HYPb1?) = 
      HYPden(HYPb1) - HYPnum(HYPb0)*HYPden(HYPb1)*HYPdenz(HYPb0,HYPb1)
    ;
    also,once HYPnumz(?a,HYPa0?,HYPa1?)*HYPdenz(HYPb0?,HYPb1?) = 
      HYPnumEval(HYPa1)*HYPden(HYPb1)*HYPaux(`Var'^nargs_(?a))
      +
      HYPnumz(?a,HYPa0 - HYPa1*HYPb0/HYPb1)*HYPdenz(HYPb0,HYPb1);
    also,once HYPnumz(?a,HYPa0?,HYPa1?,HYPa2?)*HYPdenz(HYPb0?,HYPb1?,HYPb2?) = 
      HYPnumEval(HYPa2)*HYPden(HYPb2)*HYPaux(`Var'^nargs_(?a))
      +
      HYPnumz(?a,HYPa0 - HYPa2*HYPb0/HYPb2,HYPa1 - HYPa2*HYPb1/HYPb2)*HYPdenz(HYPb0,HYPb1,HYPb2);
    #call HypGetPolyCoefficients(HYPaux,`Var',HYPn1)
    id HYPaux(1) = 1;
    id HYPnumz(HYPa0?) = HYPnumEval(HYPa0);
    id HYPnumz(HYPa0?,HYPa1?)*HYPdenz(HYPb0?,HYPb1?,HYPb2?) = HYPnumEval(HYPa0,HYPa1)*HYPdenz(HYPb0,HYPb1,HYPb2);
    If (count(HYPaux,1) > 0);
      Multiply Replace_(HYPaux,HYPnumz);
    EndIf;
  EndRepeat;
  Repeat id HYPnumEval(HYPn1?) = HYPnum(HYPeval(HYPn1));
  Repeat id HYPnumEval(HYPn1?,HYPn2?) = HYPnum(HYPeval(HYPn1),HYPeval(HYPn2));
  #Define NumArgPath "HYPnum"
  #call HypCompressCoeffs(HYPeval,HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn,HYPratep,`HYPtempPrefix',HYPtempCount,NumArgPath)
  Repeat id HYPnum(HYPn1?,HYPn2?) = HYPnumz(HYPn1,HYPn2);
#endprocedure
#procedure HypPartialFraction(NumFuncID,DenFuncID,NumCoeffFuncID,DenCoeffFuncID,SymbolID,dFlag,ParkSpectator)
  If (match(`DenFuncID'(`SymbolID'1?!{`SymbolID'3?}$PFa0,`SymbolID'2?$PFa1)*
    `DenFuncID'(`SymbolID'3?!{`SymbolID'1?}$PFb0,`SymbolID'4?$PFb1)) > 0);
    $PFdelta = $PFa1*$PFb0 - $PFa0*$PFb1;
    $PFnumTerms = nterms_($PFdelta);
    If ($PFnumTerms == 0);
      Print "We have proportional denominators...";
      Print "Basic partial fraction is not working...";
      Print "Run a normalization routine prior";
      Print "%t";
      Print "a0 : %$",$PFa0;
      Print "a1 : %$",$PFa1;
      Print "b0 : %$",$PFb0;
      Print "b1 : %$",$PFb1;
      Print "HypPartialFraction";
      Exit;
    EndIf;
    ModuleOption,Local,$PFa0,$PFa1,$PFb0,$PFb1,$PFdelta,$PFnumTerms;
    id,once,ifmatch->PFbasicMatched,`DenFuncID'($PFa0,$PFa1)*`DenFuncID'($PFb0,$PFb1) =
      `DenCoeffFuncID'($PFdelta)*
      (
        `NumFuncID'($PFa1)*`DenFuncID'($PFa0,$PFa1)
        -
        `NumFuncID'($PFb1)*`DenFuncID'($PFb0,$PFb1)
      )
    ;
  ElseIf (match(`DenFuncID'(`SymbolID'1?$PFa0,`SymbolID'2?!{`SymbolID'4?}$PFa1)*
    `DenFuncID'(`SymbolID'3?$PFb0,`SymbolID'4?!{`SymbolID'2?}$PFb1)) > 0);
    $PFdelta = $PFa1*$PFb0 - $PFa0*$PFb1;
    $PFnumTerms = nterms_($PFdelta);
    If ($PFnumTerms == 0);
      Print "We have proportional denominators...";
      Print "Basic partial fraction is not working...";
      Print "Run a normalization routine prior";
      Print "%t";
      Print "a0 : %$",$PFa0;
      Print "a1 : %$",$PFa1;
      Print "b0 : %$",$PFb0;
      Print "b1 : %$",$PFb1;
      Print "HypPartialFraction";
      Exit;
    EndIf;
    ModuleOption,Local,$PFa0,$PFa1,$PFb0,$PFb1,$PFdelta,$PFnumTerms;
    id,once,ifmatch->PFbasicMatched,`DenFuncID'($PFa0,$PFa1)*`DenFuncID'($PFb0,$PFb1) =
      `DenCoeffFuncID'($PFdelta)*
      (
        `NumFuncID'($PFa1)*`DenFuncID'($PFa0,$PFa1)
        -
        `NumFuncID'($PFb1)*`DenFuncID'($PFb0,$PFb1)
      )
    ;
  ElseIf (match(`DenFuncID'(`SymbolID'1?$PFa,1)*
    `DenFuncID'(`SymbolID'2?$PFq0,`SymbolID'3?$PFq1,1)) > 0);
    $PFdelta = $PFa^2 - $PFq1*$PFa + $PFq0;
    $PFnumTerms = nterms_($PFdelta);
    If ($PFnumTerms == 0);
      Print "We have a vanishing Q(-a)...";
      Print "Basic partial fraction is not working for Q...";
      Print "%t";
      Print "a : %$",$PFa;
      Print "q0 : %$",$PFq0;
      Print "q1 : %$",$PFq1;
      Print "HypPartialFraction";
      Exit;
    EndIf;
    id,once,ifmatch->PFbasicMatched,`DenFuncID'($PFa,1)*`DenFuncID'($PFq0,$PFq1,1) =
      `DenCoeffFuncID'($PFdelta)*
      (
        `DenFuncID'($PFa,1)
        -
        `NumCoeffFuncID'($PFq1 - $PFa, 1)*`DenFuncID'($PFq0,$PFq1,1)
      )
    ;
    ModuleOption,Local,$PFa,$PFq0,$PFq1,$PFdelta,$PFnumTerms;
  Else;
    #If ("`ParkSpectator'" != "")
      ToSpectator `ParkSpectator';
    #Endif
  EndIf;
  Goto PFbasicNotMatched;
  Label PFbasicMatched;
  $`dFlag' = 0;
  ModuleOption,Minimum,$`dFlag';
  Label PFbasicNotMatched;
#endprocedure
#procedure HypPartialFractionAll(Var,UsePark)
  #Define HYPtempUsePark "0"
  #Define HYPtempSpecName ""
  #Define HYPtempPFiterations "0"
  #Do PFiter=1,1
    #$PFisOver = 1;
    #$PFrunTowerCancel = 0;
    #call HypIncrementPreprocCounter(HYPtempPFiterations)
    #If ((`UsePark' == 1) && (`HYPtempPFiterations' == 2))
      #Define HYPtempNumActives ""
      #call HypCountListItems(HYPtempNumActives,`ActiveExprNames_')
      #If (`HYPtempNumActives' == 1)
        .sort:Creating park spec;
        #Redefine HYPtempUsePark "1"
        #Redefine HYPtempSpecName "`HYPparkSpecName'"
        #Define HYPtempMainIntegrandExpr "`ActiveExprNames_'"
        CreateSpectator `HYPparkSpecName',"`HYPparkSpecFile'";
      #EndIf
    #EndIf
    #call HypNormalizeDensToMonicForm(HYPdenz,HYPden,HYPnum,HYPaux,HYPn)
    #call HypPartialFraction(HYPnum,HYPdenz,HYPnumEval,HYPdenEval,HYPn,PFisOver,`HYPtempSpecName')
    If ($PFisOver == 0);
      id HYPdenEval(HYPn1?) = HYPden(HYPeval(HYPn1));
      If (count(HYPnumEval,1) > 0) $PFrunTowerCancel = 1;
      id HYPnumEval(HYPn1?,1) = HYPnumz(HYPeval(HYPn1),1);
    EndIf;
    ModuleOption,Maximum,$PFrunTowerCancel;
    .sort:PFiter is over;
    #If (`$PFisOver' == 0)
      #Define DenArgPath "HYPden"
      #Define NumArgPath "HYPnumz"
      #call HypCompressCoeffs(HYPeval,HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn,HYPratep,`HYPtempPrefix',HYPtempCount,DenArgPath,NumArgPath)
      #Redefine PFiter "0"
    #EndIf
    #If (`$PFrunTowerCancel' == 1)
      #call HypCancelTowers(`Var')
    #EndIf
  #EndDo
  #If (`HYPtempUsePark' == 1)
    #If (TermsIn(`HYPtempMainIntegrandExpr') > 0)
      #Write "At this point `HYPtempMainIntegrandExpr' should have been starved..."
      #Write "%e",`HYPtempMainIntegrandExpr';
      #Terminate
    #EndIf
    Drop `HYPtempMainIntegrandExpr';
    .sort:`HYPtempMainIntegrandExpr' dropped;
    CopySpectator `HYPtempMainIntegrandExpr' = `HYPparkSpecName';
  #EndIf
  #call HypNormalizeNumsDens(HYPnum,HYPden,HYPn)
  #If (`HYPtempUsePark' == 1)
    .sort:`HYPtempMainIntegrandExpr' recreated;
    RemoveSpectator `HYPparkSpecName';
  #EndIf
#endprocedure
#procedure HypPartialIntegrate(Var,NumFuncID,DenFuncID,NumCoeffFuncID,DenCoeffFuncID,LimitFuncID,AuxFuncID,DiffSymbolID,SymbolID,UpperLimit,LowerLimit,dFlag)
  If (match(`DenFuncID'(`SymbolID'1?,`SymbolID'2?)) > 1);
    If ((match(`DenFuncID'(`SymbolID'1?!{`SymbolID'3?},`SymbolID'2?)*`DenFuncID'(`SymbolID'3?!{`SymbolID'1?},`SymbolID'4?)) > 0) || 
        (match(`DenFuncID'(`SymbolID'1?,`SymbolID'2?!{`SymbolID'4?})*`DenFuncID'(`SymbolID'3?,`SymbolID'4?!{`SymbolID'2?})) > 0));
      Print "Only one of a kind linear denominators is allowed...";
      Print "Term: %t";
      Print "HypPartialIntegrate";
      Exit;
    EndIf;
    id `DenFuncID'(`SymbolID'1?,`SymbolID'2?) = `AuxFuncID'(`SymbolID'1,`SymbolID'2);
    $LFtempPowerCount = count_(`AuxFuncID',1);
    ModuleOption,Local,$LFtempPowerCount;
    Multiply Replace_(`AuxFuncID',`DenFuncID');
    id `DenFuncID'(`SymbolID'1?,`SymbolID'2?)^$LFtempPowerCount = 
      1/(1 - $LFtempPowerCount)*
        `DenCoeffFuncID'(`SymbolID'2)*
        `DenFuncID'(`SymbolID'1,`SymbolID'2)^($LFtempPowerCount-1)*
        (
          `LimitFuncID'(`UpperLimit')
          -
          `LimitFuncID'(`LowerLimit')
        )
      -
      1/(1 - $LFtempPowerCount)*
        `DenCoeffFuncID'(`SymbolID'2)*
        `DenFuncID'(`SymbolID'1,`SymbolID'2)^($LFtempPowerCount-1)*
        `DiffSymbolID'
    ;
    $`dFlag' = 0;
    ModuleOption,Minimum,$`dFlag';
  ElseIf ((match(`DenFuncID'(`SymbolID'1?,`SymbolID'2?,?a)) == 0) && (count(`Var',1) >= 0));
      Repeat id `NumFuncID'(`SymbolID'1?,`SymbolID'2?,?a) =
        `NumCoeffFuncID'(`SymbolID'1) + `Var'*`NumFuncID'(`SymbolID'2,?a);
      id `NumFuncID'(`SymbolID'1?) = `NumCoeffFuncID'(`SymbolID'1);
      id `NumCoeffFuncID'(`SymbolID'1?number_) = `SymbolID'1;
      $LFtempPowerCount = count_(`Var',1);
      ModuleOption,Local,$LFtempPowerCount;
      id `Var'^$LFtempPowerCount = 
        1/(1 + $LFtempPowerCount)*
          `Var'^($LFtempPowerCount+1)*
          (
            `LimitFuncID'(`UpperLimit')
            -
            `LimitFuncID'(`LowerLimit')
          )
        -
        1/(1 + $LFtempPowerCount)*
          `Var'^($LFtempPowerCount+1)*
          `DiffSymbolID'
      ;
    $`dFlag' = 0;
    ModuleOption,Minimum,$`dFlag';
  EndIf;
#endprocedure
#procedure HypDropVanishingLimitTerms(Var,NumFuncID,DenFuncID,LfuncID,AuxFuncID,DegSymbol,SymbolID,LimitFuncID,InfinitySymbolID,ZeroSymbolID)
  If (match(`LimitFuncID'(`InfinitySymbolID')) > 0);
    #call HypIsZeroAtInfinityLimit(`Var',`NumFuncID',`DenFuncID',`DegSymbol',IsZero)
    If ($IsZero == 1) Discard;
  ElseIf (match(`LimitFuncID'(`ZeroSymbolID')) > 0);
    #call HypIsZeroAtZeroLimit(`Var',`NumFuncID',`DenFuncID',`LfuncID',`AuxFuncID',`DegSymbol',`SymbolID',IsZero)
    If ($IsZero == 1) Discard;
  EndIf;
#endprocedure
#procedure HypPartialIntegrateAll(Var,UpperLimitSymbol,LowerLimitSymbol)
  #Do PartIntIter=1,1
    #$PIiter = 1;
    #$PFiter = 1;
    #call HypPartialIntegrate(`Var',HYPnumz,HYPdenz,HYPnum,HYPden,HYPlim,HYPaux,HYPdiffMark,HYPn,`UpperLimitSymbol',`LowerLimitSymbol',PIiter)
    #call HypDropVanishingLimitTerms(`Var',HYPnumz,HYPdenz,HYPLx,HYPaux,HYPdegree,HYPn1,HYPlim,HYPinf,HYPzero)
    #If ("`LowerLimitSymbol'" == "0" || ("`LowerLimitSymbol'" == "HYPzero"))
      If ((match(HYPlim(0)) > 0) || (match(HYPlim(HYPzero)) > 0));
        If ((count(`Var',1) >= 0) && (match(HYPdenz(0,?a)) == 0));
          #call HypTrivialZeroLimitNumsDens(HYPnumz,HYPdenz,HYPnum,HYPden,HYPn1)
          #call HypNormalizeNumsDens(HYPnum,HYPden,HYPn)
          #call HypTrivialZeroLimitLogs(HYPlogz,HYPlog,HYPn1)
          id HYPlog(1) = 0;
        EndIf;
      EndIf;
    #EndIf
    #call HypStashLimitTerms(HYPlim,HYPnum,HYPden,`UpperLimitSymbol',`LowerLimitSymbol',HYPn,HYPupperSpec,HYPlowerSpec)
    #call HypDiffMarkedTerms(`Var',HYPdiffMark,HYPdiffep,HYPnumz,HYPdenz,HYPnum,HYPpoly,HYPinvPoly,HYPlogDiff,HYPaux,HYPlogz,HYPLx,HYPLinfRegInfZero,HYPn)
    #call HypNormalizeLogDiffs(`Var',HYPlogDiff,HYPn)
    #call HypWrapArgs(HYPeval,HYPlogDiff,)
    #Define LogDiffArgPath "HYPlogDiff"
    #call HypCompressCoeffs(HYPeval,HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn,HYPratep,`HYPtempPrefix',HYPtempCount,LogDiffArgPath)
    #call HypNormalizeLogDiffs(`Var',HYPlogDiff,HYPn)
    .sort
    PolyRatFun;
    .sort
    #Define HYPtempOldExtras "`ExtraSymbols_'"
    #Define LogCase "HYPlogDiff"
    #call HypRestoreExtraSymbolsAsCoeffsByVar(`Var',HYPpoly,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,`HYPtempPrefix',HYPtempCount,HYPfact,HYPwrap,HYPaux,HYPmark,HYPtag,HYPvars,HYPsubstTbl,HYPn1,HYPn2,HYPn3,LogCase)
    #If ({`ExtraSymbols_'-`HYPtempOldExtras'} > 0)
      #call HypFillVarDepTable(`HYPtempPrefix',HYPtempCount,`HYPtempOldExtras',`ExtraSymbols_',HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn1,HYPn2)
    #EndIf
    .sort
    PolyRatFun HYPratep;
    .sort
    #call HypEvalLogDiffs(`Var',HYPlogDiff,HYPdenz,HYPnumz,HYPden,HYPnum,HYPpoly,HYPaux,HYPn)
    #call HypCancelTowers(`Var')
    #call HypPartialFractionAll(`Var',`HYPuseParking')
    .sort:PartInt iter over;
    #If (`$PIiter' == 0)
      #Redefine PartIntIter "0"
    #EndIf
  #EndDo
#endprocedure
#procedure HypSimplifyEmptyHyperlog(LfuncID,Var)
  #If ("`Var'" != "")
    id `LfuncID'(`Var') = 1;
  #Else
    id `LfuncID' = 1;
  #EndIf
#endprocedure
#procedure HypLoadHyperlogTables()
  #include- `HYPregLtoMZVtables'
  .sort:MZV tables loaded;
  #Redefine HYPtablesLoaded "1"
#endprocedure
#procedure HypSimplifyNumericHyperlogs(RegLfuncID,SymbolID)
  #If (`HYPtablesLoaded' == 0)
    #call HypLoadHyperlogTables
  #EndIf
  id `RegLfuncID' = 1;
  #Do iWeight=1,`MAXMZVWORDLENGTH'
    #If (`iWeight' == 1)
      #Define HYPtempRange "`SymbolID'`iWeight'?{-1,0}"
      #Define HYPtempSubstRange "`SymbolID'`iWeight'"
    #Else
      #Redefine HYPtempRange "`HYPtempRange',`SymbolID'`iWeight'?{-1,0}"
      #Redefine HYPtempSubstRange "`HYPtempSubstRange',`SymbolID'`iWeight'"
    #EndIf
    id `RegLfuncID'(`HYPtempRange') = `HYPregLtoMZVtable'`iWeight'(`HYPtempSubstRange');
  #EndDo
  If (match(`RegLfuncID'(?a)) != match(`RegLfuncID'(?a,`SymbolID'1?!{-1,0},?b)));
    Print "Substitution was incomplete possible hidden MZVs are still present";
    Print "TERM: %t";
    Print "HypSimplifyNumericHyperlogs";
    Exit;
  EndIf;
#endprocedure
#procedure HypNormal()
  If (count(HYPratep,1) == 0);
    Print "Term without HYPratep, do we have PolyRatFun activated?";
    Print "TERM: %t";
    Print "HypNormal";
    Exit;
  EndIf;
  If (count(HYPtag,1) > 0);
    Print "HYPtag on entry: it is the index head of HypNormal...";
    Print "TERM: %t";
    Print "HypNormal";
    Exit;
  EndIf;
  #call HypRollNumsDensToPolyRatFun(HYPratep,HYPnum,HYPden,HYPn)
  #call HypRollSymbolsToPolyRatFun(HYPratep,HYPn,HYPratFunExceptions)
  .sort:Syms,nums,dens to ratfunc;
  #call HypRollOutSymbolCoeffs(HYPratep,HYPnum,HYPden,HYPcoeff,HYPep,HYPn)
  id HYPden(HYPn1?symbol_) = 1/HYPn1;
  #call HypFactorizeWrappedArgs(HYPnum,HYPn,)
  .sort:num factorized;
  PolyRatFun HYPratep;
  .sort:PolyRat garbage collect;
  PolyRatFun;
  #Define HYPtempOldExtras ""
  #Define HYPtempNewExtras ""
  #call HypWrappedArgsToExtraSymbolNumbers(HYPnum,HYPtag,HYPtempOldExtras,HYPtempNewExtras,HYPn,)
  #Define HYPtempExtrasExpr ""
  #Define HYPtempDropExtras "1"
  #call HypHideActiveExpressions
  .sort:Actives hidden;
  #call HypExtraSymbolNumbersToExpression(HYPtempExtrasExpr,HYPtag,HYPaux,HYPn,`HYPtempOldExtras',`HYPtempNewExtras',`HYPtempPrefix',HYPtempCount,`HYPtempDropExtras')
  #Define HYPtempMaxTermSizePercentage "75"
  #call HypCollectBracketsToExtraSymbols(`HYPtempExtrasExpr',HYPtag,HYPcollect,HYPoverflow,HYPratep,HYPn,`HYPtempMaxTermSizePercentage')
  #If ({`ExtraSymbols_'-`HYPtempOldExtras'} > 0)
    #If ("`HYPvarDepVars'" == "")
      #Write "HYPvarDepVars is empty: new letters would get no variable dependence..."
      #Write "Have to start the calculation with a call to HypConvertToExtraSymbolForm"
      #Write "HypNormal"
      #Terminate
    #EndIf
    #call HypFillVarDepTable(TempExprPrefix,HYPtempCount,`HYPtempOldExtras',`ExtraSymbols_',HYPtag,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn1,HYPn2)
  #EndIf
  .sort
  ClearTable HYPnormalTbl;
  #call HypStoreExpressionInTableWithTag(`HYPtempExtrasExpr',HYPtag,HYPnormalTbl,1)
  .sort
  #call HypUnhideActiveExpressions
  .sort
  PolyRatFun HYPratep;
  If (match(HYPtag(HYPn1?{>`HYPtempNewExtras'})) > 0);
    Print "Alien numeric extrasymbol caught...";
    Print "TERM: %t";
    Print "HypNormal";
    Exit;
  EndIf;
  id HYPtag(HYPn1?{<{`HYPtempOldExtras'+1}}) = HYPextraSymTbl(HYPn1);
  id HYPtag(HYPn1?) = HYPnormalTbl(HYPn1);
  id HYPnormalTbl(HYPn1?) = 0;
  If (count(HYPextraSymTbl,1) > 0);
    Print "Reused extra symbol not in HYPextraSymTbl...";
    Print "TERM: %t";
    Print "HypNormal";
    Exit;
  EndIf;
  .sort:Normal is over;
#endprocedure
#procedure HypSimplifyWithSchwartzZippel()
  If (count(HYPratep,1) == 0);
    Print "Term without HYPratep, do we have PolyRatFun activated?";
    Print "TERM: %t";
    Print "HypSimplifyWithSchwartzZippel";
    Exit;
  EndIf;
  #call HypRollNumsDensToPolyRatFun(HYPratep,HYPnum,HYPden,HYPn)
  #call HypRollSymbolsToPolyRatFun(HYPratep,HYPn,HYPratFunExceptions)
  .sort:Syms,nums,dens to ratfunc;
  #call HypRollOutSymbolCoeffs(HYPratep,HYPnum,HYPden,HYPcoeff,HYPep,HYPn)
  id HYPden(HYPn1?symbol_) = 1/HYPn1;
  #call HypFactorizeWrappedArgs(HYPnum,HYPn,)
  .sort:num factorized;
  PolyRatFun HYPratep;
  .sort:PolyRat garbage collect;
  PolyRatFun;
  #call HypSchwartzZippelCancel(HYPnum,HYPeval,HYPaux,HYPn,HYPextraSymTbl,`HYPtempPrefix',HYPtempCount,HYPvarDepVars,HYPuserParameters,)
  SplitArg,HYPnum;
  Repeat id HYPnum(HYPn1?,HYPn2?,?a) = HYPnum(HYPn1) + HYPnum(HYPn2,?a);
  #call HypNormalizeNums(HYPnum,HYPden,HYPn)
  .sort:Nums normalized;
  PolyRatFun HYPratep;
  .sort:Schwartz-Zippel is over;
#endprocedure
#procedure HypSimpleSchwartzZippel(?Functions)
  PolyRatFun;
  id HYPratep(HYPn1?,HYPn2?) = HYPn1/HYPn2;
  Bracket HYPep,HYPratFunExceptions,`?Functions';
  .sort:Bracketed for simple S-Z;
  Keep brackets;
  $HYPtempNumTerms = TermsInBracket_;
  ModuleOption,Local,$HYPtempNumTerms;
  Multiply HYPnumTerms($HYPtempNumTerms);
  .sort:Number of terms added;
  Multiply 1 + HYPtag;
  If (Occurs(HYPtag) > 0);
    FromPolynomial;
    #call HypSchwartzZippelSubstitution(HYPvarDepVars,HYPuserParameters,HYPn)
    Argument HYPnum,HYPden,HYPratep,denom_;
      FromPolynomial;
      #call HypSchwartzZippelSubstitution(HYPvarDepVars,HYPuserParameters,HYPn)
    EndArgument;
    id HYPnum(HYPn1?number_) = HYPn1;
    id HYPden(HYPn1?number_) = 1/HYPn1;
  EndIf;
  Bracket HYPep,HYPratFunExceptions,`?Functions',HYPnumTerms;
  .sort:Numeric term added;
  Keep brackets;
  $HYPtempNumTerms = TermsInBracket_;
  ModuleOption,Local,$HYPtempNumTerms;
  If (match(HYPnumTerms($HYPtempNumTerms)) > 0);
    Discard;
  Else;
    id HYPnumTerms(HYPn1?) = 1;
  EndIf;
  .sort:Zero brackets dropped;
  PolyRatFun HYPratep;
  id HYPtag = 0;
  id HYPep^HYPn1?{>0} = HYPratep(HYPep^HYPn1,1);
  id HYPep^HYPn1?{<0} = HYPratep(1,HYPep^(-HYPn1));
  .sort:Schwartz-Zippel is over;
#endprocedure
#procedure HypDiffMarkedTerms(Var,DiffSymbolID,DiffEpsSymbolID,NumFuncID,DenFuncID,NumCoeffFuncID,PolyFuncID,InvPolyFuncID,LogDiffFuncID,AuxFuncID,LogFuncID,LfuncID,RegLfuncID,SymbolID)
  If (count(`DiffSymbolID',1) > 0);
    If (match(`LogFuncID'(`SymbolID'1?,`SymbolID'2?,`SymbolID'3?,?a)) > 0);
      Print "To differentiate only linear logs are allowed...";
      Print "Term: %t";
      Print "HypDiffMarkedTerms";
      Exit;
    EndIf;
  EndIf;
  If ((count(`DiffSymbolID',1) > 0) && (count(`LogFuncID',1,`LfuncID',1,`RegLfuncID',1) == 0)) Discard;
  If (count(`DiffSymbolID',1) > 0);
    id `LogFuncID'(`SymbolID'1?,`SymbolID'2?) = 
      `LogFuncID'(`SymbolID'1,`SymbolID'2) 
      +
      `DiffEpsSymbolID'*`NumCoeffFuncID'(`SymbolID'2)*`DenFuncID'(`SymbolID'1,`SymbolID'2)
    ;
    id `LfuncID'(`SymbolID'1?,?a) = 
      `LfuncID'(`SymbolID'1,?a) 
      +
      `DiffEpsSymbolID'*`DenFuncID'(-`SymbolID'1,1)*`LfuncID'(?a) 
    ;
    id `LfuncID' = 1;
    #call HypDiffRegLimitHlogs(`RegLfuncID',`LogDiffFuncID',`AuxFuncID',`DiffEpsSymbolID',`SymbolID')
  EndIf;
  If ((count(`DiffSymbolID',1) > 0) && ((count(`DiffEpsSymbolID',1) > 1) || (count(`DiffEpsSymbolID',1) == 0))) Discard;
  If ((count(`DiffSymbolID',1) > 0) && (count(`DiffEpsSymbolID',1) == 1));
    Multiply Replace_(`DiffSymbolID',1,`DiffEpsSymbolID',1);
  EndIf;
#endprocedure
#procedure HypDiffRegLimitHlogs(RegLfuncID,LogDiffFuncID,AuxFuncID,DiffEpsSymbolID,SymbolID)
  id `RegLfuncID'(?a,`SymbolID'1?!number_,?b) = `AuxFuncID'(?a,`SymbolID'1,?b);
  id `AuxFuncID'(?a,`SymbolID'1?!number_) = 
    `AuxFuncID'(?a,`SymbolID'1) 
    -
    `DiffEpsSymbolID'*`LogDiffFuncID'(`SymbolID'1)*`RegLfuncID'(?a)
  ;
  id `AuxFuncID'(?a) = `RegLfuncID'(?a) + `AuxFuncID'(?a);
  id,all `AuxFuncID'(?a,`SymbolID'1?!{`SymbolID'2?},`SymbolID'2?!{`SymbolID'1?},?b) = 
    `DiffEpsSymbolID'*
      `LogDiffFuncID'(`SymbolID'1 - `SymbolID'2)*
      (
        `RegLfuncID'(?a,`SymbolID'1,?b)
        -
        `RegLfuncID'(?a,`SymbolID'2,?b)
      )
  ;
  id `AuxFuncID'(?a) = 0;
  id `RegLfuncID' = 1;
#endprocedure
#procedure HypNormalizeLogDiffs(Var,LogDiffFuncID,SymbolID)
  FactArg `LogDiffFuncID';
  Repeat id `LogDiffFuncID'(`SymbolID'1?,`SymbolID'2?,?a) = 
    `LogDiffFuncID'(`SymbolID'1) + `LogDiffFuncID'(`SymbolID'2,?a)
  ;
  id `LogDiffFuncID'(1/`Var') = -`LogDiffFuncID'(`Var');
  id `LogDiffFuncID'(1/`SymbolID'1?) = -`LogDiffFuncID'(`SymbolID'1);
  If (match(`LogDiffFuncID'(`SymbolID'1?,`SymbolID'2?,?a)) > 0);
    Print "Factorization and splitting not working properly...";
    Print "Term: %t";
    Print "HypLogDerivPoly";
    Exit;
  EndIf;
#endprocedure
#procedure HypEvalLogDiffs(Var,LogDiffFuncID,DenFuncID,NumFuncID,DenCoeffFuncID,NumCoeffFuncID,PolyFuncID,AuxFuncID,SymbolID)
  id `LogDiffFuncID'(`SymbolID'1?number_) = 0;
  id `LogDiffFuncID'(`Var') = `DenFuncID'(0,1);
  Repeat;
    id,once `LogDiffFuncID'(`PolyFuncID'(`SymbolID'1?,?a)) = `DenFuncID'(`SymbolID'1,?a)*`AuxFuncID'1(1,?a)*`AuxFuncID'2;
    Repeat;
      id `AuxFuncID'1(`SymbolID'1?number_,`SymbolID'2?,?a)*`AuxFuncID'2(?b) =
        `AuxFuncID'1(`SymbolID'1 + 1,?a)*`AuxFuncID'2(?b,`SymbolID'1*`SymbolID'2)
      ;
      id `AuxFuncID'1(`SymbolID'1?number_)*`AuxFuncID'2(?b) = `AuxFuncID'3(?b);
    EndRepeat;
  EndRepeat;
  id `LogDiffFuncID'(`SymbolID'1?symbol_) = 0;
  Multiply Replace_(`AuxFuncID'3,`NumFuncID');
  #call HypNormalizeVarDepNumsDens(`NumFuncID',`DenFuncID',`NumCoeffFuncID',`DenCoeffFuncID',`SymbolID')
#endprocedure
#procedure HypDeligneLimitLogs(LfuncID,AuxFuncID,DeligneSymbolID,SymbolID)
  If (count(`AuxFuncID',1) > 0);
    Print "`AuxFuncID' could not show up because it will be used as an auxiliary...";
    Print "TERM: %t";
    Print "HypDeligneLimitLogs";
    Exit;
  EndIf;
  id `LfuncID'(?a,`SymbolID'1?!{0,},?b) = `AuxFuncID'(?a,`SymbolID'1,?b);
  id `LfuncID'(?a) = `DeligneSymbolID'^nargs_(?a)*invfac_(nargs_(?a));
  Multiply Replace_(`AuxFuncID',`LfuncID');
#endprocedure
#procedure HypApplyOneOverZToNum(VarDepNumFuncID,InvVar,SymbolID)
  Repeat id `VarDepNumFuncID'(?a,`SymbolID'1?{0,}) = `VarDepNumFuncID'(?a);
  id `VarDepNumFuncID'(?a) = (`InvVar')^(1-nargs_(?a))*`VarDepNumFuncID'(reverse_(?a));
  Repeat id `VarDepNumFuncID'(?a,`SymbolID'1?{0,}) = `VarDepNumFuncID'(?a);
#endprocedure
#procedure HypApplyOneOverZToDen(VarDepDenFuncID,InvVar,SymbolID)
  Repeat id `VarDepDenFuncID'(?a,`SymbolID'1?{0,}) = `VarDepDenFuncID'(?a);
  id `VarDepDenFuncID'(?a) = (`InvVar')^(nargs_(?a)-1)*`VarDepDenFuncID'(reverse_(?a));
  Repeat id `VarDepDenFuncID'(?a,`SymbolID'1?{0,}) = `VarDepDenFuncID'(?a);
#endprocedure
#procedure HypApplyPhiOneOverZToHyperlog(FuncID,Var,OutFuncID,InvVar,PhiFuncID,AuxFuncID,SymbolID,DoBrackets)
  #If (`DoBrackets' == 1)
    Bracket `FuncID';
    .sort
    Keep brackets;
  #EndIf
  Repeat;
    #If ("`Var'" != "")
      id,once `FuncID'(?a,`Var') = `AuxFuncID'1(?a);
    #Else
      id,once `FuncID'(?a) = `AuxFuncID'1(?a);
    #EndIf
    If (count(`AuxFuncID'1,1) > 0);
      Transform,`AuxFuncID'1,replace(1,last)=(xarg_,`PhiFuncID'(xarg_));
      Repeat;
        id `AuxFuncID'1(?a,`PhiFuncID'(0),?b) = -`AuxFuncID'1(?a,0,?b);
        also `AuxFuncID'1(?a,`PhiFuncID'(`SymbolID'1?!{0,}),?b) =
          `AuxFuncID'1(?a,1/`SymbolID'1,?b) - `AuxFuncID'1(?a,0,?b)
        ;
      EndRepeat;
      #If ("`InvVar'" != "")
        id `AuxFuncID'1(?a) = `AuxFuncID'2(?a,`InvVar');
      #Else
        id `AuxFuncID'1(?a) = `AuxFuncID'2(?a);
      #EndIf
    EndIf;
  EndRepeat;
  Multiply replace_(`AuxFuncID'2,`OutFuncID');
#endprocedure
#procedure HypResolveUpperLimitTerms(Var,VarDepNumFuncID,VarDepDenFuncID,NumFuncID,DenFuncID,LfuncID,RegLfuncID,PhiFuncID,AuxFuncID,DegSymbolID,SymbolID,UpperLimitSymbol,LowerLimitSymbol,UpperBoundSpecName,LowerBoundSpecName,DoneSpecName,TempExprPrefix,TempExprCountVar)
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  CopySpectator `TempExprPrefix'``TempExprCountVar'' = `UpperBoundSpecName';
  .sort:`UpperBoundSpecName' copied to `TempExprPrefix'``TempExprCountVar'';
  #call HypHideActiveExpressions(`TempExprPrefix'``TempExprCountVar'')
  If ((count(`Var',1) == 0) && (match(`AuxFuncID'?{`VarDepNumFuncID',`VarDepDenFuncID',`LfuncID'}(?a)) == 0));
    ToSpectator `DoneSpecName';
  EndIf;
  #If ((`UpperLimitSymbol' == HYPinf) && ((`LowerLimitSymbol' != HYPzero) && (`LowerLimitSymbol' != 0)))
    #Write "Upper limit is infinity lower limit is _not_ zero..."
    #Write "We always convert limits at infinity to limits at zero..."
    #Write "Change your integration limits accordingly!"
    #Write "HypResolveUpperLimitTerms"
    #Terminate
  #ElseIf (`UpperLimitSymbol' != HYPinf)
    #Write "At the moment only infinity is allowed as upper limit..."
    #Write "The user gave: >>>`UpperLimitSymbol'<<<"
    #Write "HypResolveUpperLimitTerms"
    #Terminate
  #EndIf
  #If (`UpperLimitSymbol' == HYPinf)
    #call HypDeconcatenateCoproduct(`LfuncID',,`LfuncID',,`RegLfuncID',,`AuxFuncID',`SymbolID')
    #call HypSimplifyEmptyHyperlog(`LfuncID',)
    #call HypSimplifyEmptyHyperlog(`RegLfuncID',)
    #call HypApplyPhiOneOverZToHyperlog(`LfuncID',,`LfuncID',,`PhiFuncID',`AuxFuncID',`SymbolID',0)
    .sort:Moebius is over;
    id `Var' = 1/`Var';
    also 1/`Var' = `Var';
    #call HypApplyOneOverZToNum(`VarDepNumFuncID',`Var',`SymbolID')
    #call HypApplyOneOverZToDen(`VarDepDenFuncID',`Var',`SymbolID')
    #call HypNormalizeNumsDens(`NumFuncID',`DenFuncID',`SymbolID')
  #EndIf
  #If ("`LowerLimitSymbol'" == "0" || ("`LowerLimitSymbol'" == "HYPzero"))
    If ((count(`Var',1) >= 0) && (match(`VarDepDenFuncID'(0,?a)) == 0));
      #call HypIsZeroAtZeroLimit(`Var',`VarDepNumFuncID',`VarDepDenFuncID',`LfuncID',`AuxFuncID',`DegSymbolID',`SymbolID'1,IsZero)
      If ($IsZero == 1) Discard;
      #call HypTrivialZeroLimitNumsDens(`VarDepNumFuncID',`VarDepDenFuncID',`NumFuncID',`DenFuncID',`SymbolID'1)
      #call HypNormalizeNumsDens(`NumFuncID',`DenFuncID',`SymbolID')
    EndIf;
    .sort:Upper limit applied;
    If ((count(`Var',1) == 0) && (match(`AuxFuncID'?{`VarDepNumFuncID',`VarDepDenFuncID',`LfuncID'}(?a)) == 0));
      ToSpectator `DoneSpecName';
    Else;
      ToSpectator `LowerBoundSpecName';
    EndIf;
  #EndIf
  .sort:After upper limit routing;
  #If (TermsIn(`TempExprPrefix'``TempExprCountVar'') > 0)
    #Write "Terms still present in `TempExprPrefix'``TempExprCountVar''"
    #Write "Not all the limits are taken at `UpperLimitSymbol'"
    #Write "The expression: %e",`TempExprPrefix'``TempExprCountVar'';
    #Write "HypResolveUpperLimitTerms"
    #Terminate
  #EndIf
  .sort
  EmptySpectator `UpperBoundSpecName';
  Drop `TempExprPrefix'``TempExprCountVar'';
  .sort
  #call HypUnhideActiveExpressions
#endprocedure
#procedure HypResolveLowerLimitTerms(Var,VarDepNumFuncID,VarDepDenFuncID,LfuncID,RegLfuncID,DenFuncID,NumFuncID,CoeffFuncID,MarkFuncID,AuxFuncID,SymbolID,LowerLimitSymbol,DeligneSymbol,LowerBoundSpecName,DoneSpecName,TempExprPrefix,TempExprCountVar,ZeroLimitTableID,ZeroLimitSetFileName,WrapFuncID)
  #call HypIncrementPreprocCounter(`TempExprCountVar')
  #Define HYPtempLowerLimitExpr "`TempExprPrefix'``TempExprCountVar''"
  CopySpectator `HYPtempLowerLimitExpr' = `LowerBoundSpecName';
  .sort:`LowerBoundSpecName' copied to `HYPtempLowerLimitExpr';
  #call HypHideActiveExpressions(`HYPtempLowerLimitExpr')
  #If ((`LowerLimitSymbol' == HYPzero) || (`LowerLimitSymbol' == 0))
    #call HypSubstituteExtraSymbolZeroLimits(`ZeroLimitTableID',`ZeroLimitSetFileName',`RegLfuncID',`WrapFuncID',`SymbolID')
    #If (`HYPuseSubstitutionTables' == 1)
      #call HypSimplifyNumericHyperlogs(`RegLfuncID',`SymbolID')
    #EndIf
    id `VarDepDenFuncID'(0,`SymbolID'1?) = `DenFuncID'(`SymbolID'1)*`Var'^-1;
    #call HypShuffleHyperlogToNonZeroEnd(`LfuncID',`MarkFuncID',`AuxFuncID',`SymbolID')
    #call HypDeligneLimitLogs(`LfuncID',`AuxFuncID',`DeligneSymbol',`SymbolID')
    #call HypExpandHyperlogs(`Var',`LfuncID',`DenFuncID',`CoeffFuncID',`SymbolID')
    #call HypNormalizeNumsDens(`NumFuncID',`DenFuncID',`SymbolID')
    #If (`HYPuseNormal' == 0)
      id `Var'^`SymbolID'1?{>0} = HYPnumz(0,1)^`SymbolID'1;
      id `Var'^`SymbolID'1?{<0} = HYPdenz(0,1)^(-`SymbolID'1);
      .sort:Before simple S-Z;
      #call HypSimpleSchwartzZippel(HYPnumz,HYPdenz,HYPlog,HYPLx,HYPLinfRegInfZero)
      Repeat;
        id HYPnumz(0,1) = `Var';
        id HYPdenz(0,1) = 1/`Var';
      EndRepeat;
      .sort:After S-Z;
    #EndIf
  #EndIf
  If ((occurs(`Var') == 0) && (match(`AuxFuncID'?{`VarDepNumFuncID',`VarDepDenFuncID',`LfuncID'}(?a)) == 0));
    ToSpectator `DoneSpecName';
  EndIf;
  .sort:Lower limits applied;
  #If (TermsIn(`HYPtempLowerLimitExpr') > 0)
    #Write "Terms still present in `HYPtempLowerLimitExpr'"
    #Write "Not all the limits are taken at `LowerLimitSymbol'"
    #Write "The expression: %e",`HYPtempLowerLimitExpr';
    #Write "HypResolveLowerLimitTerms"
    #Terminate
  #EndIf
  .sort
  EmptySpectator `LowerBoundSpecName';
  Drop `HYPtempLowerLimitExpr';
  .sort
  #call HypUnhideActiveExpressions
#endprocedure
#procedure HypRetrieveDoneTerms(ExprID,DoneSpecName)
  #If (Exists(`ExprID') == 1)
    #If (TermsIn(`ExprID') != 0)
      #Write "`ExprID' is present and not empty..."
      #Write "When spectator is dumped onto it original terms are lost..."
      #Write "HypRetrieveDoneTerms"
      #Terminate
    #Else
      #Write "`ExprID' is present..."
      #Write "A spectator cannot be copied onto an already present expression..."
      #Write "HypRetrieveDoneTerms"
      #Terminate
    #EndIf
  #EndIf
  CopySpectator `ExprID' = `DoneSpecName';
  .sort:`DoneSpecName' -> `ExprID';
  EmptySpectator `DoneSpecName';
  .sort:`DoneSpecName' emptied;
#endprocedure
#procedure HypBuildPrimitive(Var,VarDepNumFuncID,VarDepDenFuncID,NumFuncID,DenFuncID,LfuncID,LimitFuncID,AuxFuncID,SymbolID,UpperLimitSymbol,LowerLimitSymbol)
  If ((count(`Var',1) == 0) && (match(`AuxFuncID'?{`VarDepNumFuncID',`VarDepDenFuncID',`LfuncID'}(?a)) == 0));
    Print "Primitive function requested for term without any `Var' dependent function...";
    Print "TERM: %t";
    Print "HypBuildPrimitive";
    Exit;
  EndIf;
  If ((match(`VarDepDenFuncID'(`SymbolID'1?!{`SymbolID'3?},`SymbolID'2?)*
             `VarDepDenFuncID'(`SymbolID'3?!{`SymbolID'1?},`SymbolID'4?)) > 0) ||
      (match(`VarDepDenFuncID'(`SymbolID'1?,`SymbolID'2?!{`SymbolID'4?})*
             `VarDepDenFuncID'(`SymbolID'3?,`SymbolID'4?!{`SymbolID'2?})) > 0));
    Print "Partial fraction is required before this routine...";
    Print "TERM: %t";
    Print "HypBuildPrimitive";
    Exit;
  EndIf;
  If (count(`LfuncID',1) > 1);
    Print "During integration only one `LfuncID' is allowed per term, run shuffle...";
    Print "TERM: %t"; 
    Print "HypBuildPrimitive";
    Exit;
  EndIf;
  If (match(`LfuncID'(?a)) == 0);
    id `VarDepDenFuncID'(`SymbolID'1?,`SymbolID'2?) =
      (`LimitFuncID'(`UpperLimitSymbol') - `LimitFuncID'(`LowerLimitSymbol'))*
      `DenFuncID'(`SymbolID'2)*`LfuncID'(-`SymbolID'1/`SymbolID'2)
    ;
  ElseIf (match(`LfuncID'(?a)) == 1);
    id `VarDepDenFuncID'(`SymbolID'1?,`SymbolID'2?)*`LfuncID'(?a) =
      (`LimitFuncID'(`UpperLimitSymbol') - `LimitFuncID'(`LowerLimitSymbol'))*
      `DenFuncID'(`SymbolID'2)*`LfuncID'(-`SymbolID'1/`SymbolID'2,?a)
    ;
  EndIf;
  #call HypNormalizeDens(`DenFuncID',`NumFuncID',`SymbolID')
#endprocedure
#procedure HypGetLeadingDegree(Var,dDegree)
  $HYPtempDegree = count_(`Var',1);
  ModuleOption,Local,$HYPtempDegree;
  If ($HYPtempDegree < $`dDegree') $`dDegree' = $HYPtempDegree;
#endprocedure
#procedure HypWrapTermsWithGivenDegree(Var,dDegree,WrapFuncID)
  If (count(`Var',1) == $`dDegree') PutInside `WrapFuncID';
#endprocedure
#procedure HypKeepRightMostWrapper(FuncID,WrapFuncID,SymbolID)
  Repeat;
    id `FuncID'(?a,`WrapFuncID'(`SymbolID'1?),?b,`WrapFuncID'(`SymbolID'2?),?c) = 
      `FuncID'(?a,`SymbolID'1,?b,`WrapFuncID'(`SymbolID'2),?c)
    ;
  EndRepeat;
#endprocedure
#procedure HypShuffleMarkedArgToRight(FuncID,OutFuncID,RHSprodFuncID,MarkFuncID,AuxFuncID,SymbolID)
  If (match(`FuncID'(?a,`MarkFuncID'(?b),?c,`MarkFuncID'(?d),?e)) > 0);
    Print "This routine is only working with function with exactly one `MarkFuncID' arg";
    Print "TERM: %t";
    Print "HypShuffleMarkedArgToRight";
    Exit;
  EndIf;
  Label ZeroLimitShuffleLabel;
  Term;
    id,once `FuncID'(?a,`MarkFuncID'(?b),`SymbolID'1?,?c) =
      `OutFuncID'(?a,`MarkFuncID'(?b))*`RHSprodFuncID'(`SymbolID'1,?c)
      - 
      `AuxFuncID'1(?a,`MarkFuncID'(?b))*`AuxFuncID'1(`SymbolID'1,?c)
      + 
      `AuxFuncID'1(?a,`MarkFuncID'(?b),`SymbolID'1,?c)
    ;
    Shuffle `AuxFuncID'1;
    Multiply Replace_(`AuxFuncID'1,`FuncID');
    sort;
  EndTerm;
  If (match(`FuncID'(?a,`MarkFuncID'(?b),`SymbolID'1?,?c)) > 0) Goto ZeroLimitShuffleLabel;
  #If ("`FuncID'" != "`OutFuncID'")
    id `FuncID'(?a,`MarkFuncID'(`SymbolID'1?)) = `OutFuncID'(?a,`MarkFuncID'(`SymbolID'1));
  #EndIf
#endprocedure
#procedure HypRescaleAndLimitWithLeadingDegree(Var,FuncID,LeadFuncID,AuxFuncID,SymbolID)
  If ((count(`FuncID',1) > 0) && (match(`FuncID'(?a,`LeadFuncID'(`SymbolID'1?))) == 0));
    Print "Only works if last argument of `FuncID' is wrapped by `LeadFuncID'...";
    Print "TERM: %t";
    Print "HypRescaleAndLimitWithLeadingDegree";
    Exit;
  EndIf;
  Repeat;
    If (match(`FuncID'(?a,`LeadFuncID'(`SymbolID'1?))) > 0);
      id,once `FuncID'(?a,`LeadFuncID'(`SymbolID'1?$HYPtempLast)) = `AuxFuncID'1(?a,`SymbolID'1);
      ModuleOption,Local,$HYPtempLast;
      Inside $HYPtempLast;
        $HYPtempDegree = `Var'^(-count_(`Var',1));
        ModuleOption,Local,$HYPtempDegree;
      EndInside;
      Argument `AuxFuncID'1;
        Multiply $HYPtempDegree;
      EndArgument;
      Argument `AuxFuncID'1;
        If (count(`Var',1) < 0);
          Print "Negative power of `Var' after rescaling...";
          Print "Last arguments degree was not the leading: %$",$HYPtempDegree;
          Print "ARG: %t";
          Exit "HypRescaleAndLimitWithLeadingDegree";
        EndIf;
      EndArgument;
      Argument `AuxFuncID'1;
        Multiply Replace_(`Var',0);
      EndArgument;
      Multiply Replace_(`AuxFuncID'1,`AuxFuncID'2);
    EndIf;
  EndRepeat;
  Multiply Replace_(`AuxFuncID'2,`FuncID');
#endprocedure
#procedure HypSubstituteExtraSymbolZeroLimits(ZeroLimitTableID,ZeroLimitSetFileName,FuncID,WrapFuncID,SymbolID)
  Argument `FuncID';
    #call HypSymbolsToNumbersInFuncArgWithSetInFile(`ZeroLimitSetFileName',`WrapFuncID',`SymbolID')
  EndArgument;
  Multiply Replace_(`WrapFuncID',`ZeroLimitTableID');
#endprocedure
#procedure HypChangeVarDepFunctionHeads(Var,FuncID,DepFuncID,IndepFuncID,AuxFuncID)
  id `FuncID'(?a) = `FuncID'(`AuxFuncID'1(?a));
  Argument `FuncID';
    If (Occurs(`Var') > 0);
      Multiply Replace_(`AuxFuncID'1,`DepFuncID');
    Else;
      Multiply Replace_(`AuxFuncID'1,`IndepFuncID');
    EndIf;
  EndArgument;
  id `FuncID'(`AuxFuncID'1?{`DepFuncID',`IndepFuncID'}(?a)) = `AuxFuncID'1(?a);
#endprocedure
#procedure HypZeroLimitRegLimitHlogs(LinfFuncID,ZeroLimitLinfFuncID,ToZeroLimitLinfFuncID,ToLinfFuncID,Var,LeadFuncID,AuxFuncID,SymbolID)
  #call HypChangeVarDepFunctionHeads(`Var',`LinfFuncID',`LinfFuncID',`ZeroLimitLinfFuncID',`AuxFuncID')
  Repeat;
    If (count(`LinfFuncID',1) > 0);
      Repeat;
        If (count(`LinfFuncID',1) > 0);
          id,once `LinfFuncID'(?a) = `AuxFuncID'1(?a);
          $Degree = 99;
          ModuleOption,Local,$Degree;
          Argument `AuxFuncID'1;
            #call HypGetLeadingDegree(`Var',Degree)
          EndArgument;
          Argument `AuxFuncID'1;
            #call HypWrapTermsWithGivenDegree(`Var',Degree,`LeadFuncID')
          EndArgument;
          Multiply Replace_(`AuxFuncID'1,`AuxFuncID'2);
        EndIf;
      EndRepeat;
      Multiply Replace_(`AuxFuncID'2,`LinfFuncID');
      #call HypKeepRightMostWrapper(`LinfFuncID',`LeadFuncID',`SymbolID')
      #call HypShuffleMarkedArgToRight(`LinfFuncID',`ToZeroLimitLinfFuncID',`ToLinfFuncID',`LeadFuncID',`AuxFuncID',`SymbolID')
      #call HypChangeVarDepFunctionHeads(`Var',`ToLinfFuncID',`LinfFuncID',`ToZeroLimitLinfFuncID',`AuxFuncID')
      Multiply Replace_(`ToLinfFuncID',`LinfFuncID');
    EndIf;
  EndRepeat;
  #call HypRescaleAndLimitWithLeadingDegree(`Var',`ToZeroLimitLinfFuncID',`LeadFuncID',`AuxFuncID',`SymbolID')
  Multiply Replace_(`ToZeroLimitLinfFuncID',`ZeroLimitLinfFuncID');
#endprocedure
#procedure HypDiffLogStepInBootStrap(ContainerFuncID,LvarFuncID,ZeroLimitRegLfuncID,LvarWeightFuncID,RegLweightFuncID,LogDiffFuncID,AuxFuncID,SymbolID)
  If (count(`ContainerFuncID',1) > 1);
    Print "Only one `ContainerFuncID' allowed per term...";
    Print "TERM: %t";
    Print "HypDiffLogStepInBootStrap";
    Exit;
  ElseIf (count(`RegLweightFuncID',1) > 0);
    Print "No `RegLweightFuncID' function is allowed on top level at the beginning...";
    Print "TERM: %t";
    Print "HypDiffLogStepInBootStrap";
    Exit;
  EndIf;
  id `ContainerFuncID'(`LvarWeightFuncID'(?a),`RegLweightFuncID'(?b,`SymbolID'1?)) = 
    `ContainerFuncID'(`LvarWeightFuncID'(?a),`RegLweightFuncID'(?b,`SymbolID'1))
    +
    `ContainerFuncID'(`LvarWeightFuncID'(?a))*`AuxFuncID'1(?b,`SymbolID'1)
    +
    `LvarFuncID'(?a)*`ZeroLimitRegLfuncID'(?b,`SymbolID'1)
  ;
  id `ContainerFuncID'(`LvarWeightFuncID'(?a),`RegLweightFuncID'(?b,`SymbolID'1?!number_)) = 
    -
    `LogDiffFuncID'(`SymbolID'1)*`ContainerFuncID'(`LvarWeightFuncID'(?a),`RegLweightFuncID'(?b))
  ;
  also `ContainerFuncID'(`LvarWeightFuncID'(?a),`RegLweightFuncID'(?b,`SymbolID'1?number_)) = 0;
  id `AuxFuncID'1(?a,`SymbolID'1?!{`SymbolID'2?},?b,`SymbolID'2?!{`SymbolID'1?},?c) =
    `RegLweightFuncID'(?a,`SymbolID'1,?b,`SymbolID'2,?c)
  ;
  id `AuxFuncID'1(?a) = 0;
  id,all `RegLweightFuncID'(?a,`SymbolID'1?!{`SymbolID'2?},`SymbolID'2?!{`SymbolID'1?},?b) = 
    `LogDiffFuncID'(`SymbolID'1 - `SymbolID'2)*
    (
      `RegLweightFuncID'(?a,`SymbolID'1,?b)
      -
      `RegLweightFuncID'(?a,`SymbolID'2,?b)
    )
  ;
  id `ContainerFuncID'(`LvarWeightFuncID'(?a))*`RegLweightFuncID'(?b) =
    `ContainerFuncID'(`LvarWeightFuncID'(?a),`RegLweightFuncID'(?b))
  ;
#endprocedure
#procedure HypDiffLogsToLetters(Var,ContainerFuncID,LvarWeightFuncID,RegLweightFuncID,LogDiffFuncID,PolyFuncID,EvalFuncID,MarkFuncID,RatFuncID,FactFuncID,WrapFuncID,TagFuncID,VarsFuncID,AuxFuncID,VarDepTableID,VarsList,NumberOfLettersVar,SubstitutionTableID,TempExprPrefix,TempExprCountVar,SymbolID)
  #call HypNormalizeLogDiffs(`Var',`LogDiffFuncID',`SymbolID')
  #call HypWrapArgs(`EvalFuncID',`LogDiffFuncID',)
  #Define LogDiffArgPath "`LogDiffFuncID'"
  #call HypCompressCoeffs(`EvalFuncID',`MarkFuncID',`VarDepTableID',`VarsList',`NumberOfLettersVar',`SymbolID',`RatFuncID',`TempExprPrefix',`TempExprCountVar',LogDiffArgPath)
  #call HypNormalizeLogDiffs(`Var',`LogDiffFuncID',`SymbolID')
  #If ("`RatFuncID'" != "")
    .sort
    PolyRatFun;
  #EndIf
  .sort
  #Define HYPtempOldExtras "`ExtraSymbols_'"
  #Define LogCase "`LogDiffFuncID'"
  #call HypRestoreExtraSymbolsAsCoeffsByVar(`Var',`PolyFuncID',`VarDepTableID',`VarsList',`NumberOfLettersVar',`TempExprPrefix',`TempExprCountVar',`FactFuncID',`WrapFuncID',`AuxFuncID',`MarkFuncID',`TagFuncID',`VarsFuncID',`SubstitutionTableID',`SymbolID'1,`SymbolID'2,`SymbolID'3,LogCase)
  #If ({`ExtraSymbols_'-`HYPtempOldExtras'} > 0)
    #call HypFillVarDepTable(`TempExprPrefix',`TempExprCountVar',`HYPtempOldExtras',`ExtraSymbols_',`MarkFuncID',`VarDepTableID',`VarsList',`NumberOfLettersVar',`SymbolID'1,`SymbolID'2)
  #EndIf
  #If ("`RatFuncID'" != "")
    .sort
    PolyRatFun `RatFuncID';
  #EndIf
  .sort
  If (match(`LogDiffFuncID'(`PolyFuncID'(`SymbolID'1?,`SymbolID'2?,`SymbolID'3?,?a))) > 0);
    Print "At the moment only linear factors can be converted to letters...";
    Print "Try to rationalize it!";
    Print "HypDiffLogsToLetters";
    Exit;
  EndIf;
  id `LogDiffFuncID'(`SymbolID'1?number_) = 0;
  also `LogDiffFuncID'(`Var')*`ContainerFuncID'(`LvarWeightFuncID'(?a),?b) =
    `ContainerFuncID'(`LvarWeightFuncID'(?a,0),?b)
  ;
  also `LogDiffFuncID'(`PolyFuncID'(`SymbolID'1?,`SymbolID'2?))*`ContainerFuncID'(`LvarWeightFuncID'(?a),?b) =
    `ContainerFuncID'(`LvarWeightFuncID'(?a,-`SymbolID'1/`SymbolID'2),?b)
  ;
  also `LogDiffFuncID'(`SymbolID'1?) = 0;
#endprocedure
#procedure HypCreateVarDepExtrasymbolExpr(Var,VarDepExtrasExprIdVar)
  #If ("``VarDepExtrasExprIdVar''" != "")
    #Write "We assume that no `Var' reveal happened"
    #Write "NOTE: if used for a different variable you need to purge `VarDepExtrasExprIdVar' as well..."
    #Write "HypCreateVarDepExtrasymbolExpr"
    #Terminate
  #EndIf
  .sort:Creating VarDep expr;
  PolyRatFun;
  #call HypVarDepExtraSymbolsToExpression(`Var',0,`HYPnumExtraSymbolLetters',HYPvarDepTbl,HYPvarDepVars,`HYPtempPrefix',HYPtempCount,`VarDepExtrasExprIdVar',1)
  .sort:Var-dep extras in expr;
  PolyRatFun HYPratep;
  Hide ``VarDepExtrasExprIdVar'';
#endprocedure
#procedure HypFillExtraSymbolZeroLimitTable(Var,VarDepExtrasExprID,LimitFuncID,EvalFuncID,MarkFuncID,TagFuncID,SymbolID,VarDepTblID,ZeroLimitTableID,VarListVar,NumberOfLettersVar,TempExprPrefix,TempExprCountVar,ZeroListDumpFileName,ZeroLimitSetFileName)
  .sort:Takings limits in extras;
  Unhide `VarDepExtrasExprID';
  #call HypHideActiveExpressions
  .sort
  #call HypExpressionToDynamicSetInFile(`VarDepExtrasExprID',`ZeroListDumpFileName',`ZeroLimitSetFileName')
  #call HypCountTermsInExpression(`VarDepExtrasExprID',`TagFuncID',ExtraCount)
  #call HypExtraSymbolZeroLimitsToArgs(`VarDepExtrasExprID',`Var',`LimitFuncID',`EvalFuncID',`MarkFuncID',`SymbolID',`VarDepTblID',`VarListVar',`NumberOfLettersVar',`TempExprPrefix',`TempExprCountVar')
  id `SymbolID'1?symbol_*`LimitFuncID'(`SymbolID'2?) = `SymbolID'2;
  #call HypStoreExpressionInTableWithTag(`VarDepExtrasExprID',`TagFuncID',`ZeroLimitTableID',1)
  .sort:`ZeroLimitTableID' got filled;
  #call HypUnhideActiveExpressions
  .sort:VarDep extra limits stored;
#endprocedure
#procedure HypBootstrapRegLimitHlogs(Var,VarDepExtrasExprID)
  .sort:Turning off PolyRatFun;
  PolyRatFun;
  #call HypTagVarDepFuncsThruExtras(`Var',HYPLinfRegInfZero,HYPregL,HYPwrap,HYPaux,`VarDepExtrasExprID')
  #call HypFillExtraSymbolZeroLimitTable(`Var',`VarDepExtrasExprID',HYPlim,HYPeval,HYPmark,HYPtag,HYPn,HYPvarDepTbl,HYPzeroLimitSubstTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,`HYPtempPrefix',HYPtempCount,zerolimitdump.txt,zerolimitsubst.h)
  PolyRatFun HYPratep;
  #Do iBoot=1,1
    id,once HYPregL(?a) = HYPcontainer(HYPweightLx,HYPweightRegL(?a));
    #Do iLetter=1,1
      #call HypDiffLogStepInBootStrap(HYPcontainer,HYPLx,HYPLlimZero,HYPweightLx,HYPweightRegL,HYPlogDiff,HYPaux,HYPn)
      .sort:Bracketing for zero limit;
      Bracket HYPLlimZero;
      .sort:Bracketed in HYPLlimZero;
      Keep Brackets;
      #call HypSubstituteExtraSymbolZeroLimits(HYPzeroLimitSubstTbl,zerolimitsubst.h,HYPLlimZero,HYPlim,HYPn)
      #call HypZeroLimitRegLimitHlogs(HYPLlimZero,HYPLinfRegInfZero,HYPtoLinfRegInfZero,HYPtoLlimZero,`Var',HYPlead,HYPaux,HYPn)
      .sort:Zero limits applied;
      #call HypDiffLogsToLetters(`Var',HYPcontainer,HYPweightLx,HYPweightRegL,HYPlogDiff,HYPpoly,HYPeval,HYPmark,HYPratep,HYPfact,HYPwrap,HYPtag,HYPvars,HYPaux,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPsubstTbl,`HYPtempPrefix',HYPtempCount,HYPn)
      id HYPcontainer(HYPweightLx(?a),HYPweightRegL) = HYPLx(?a);
      id HYPLx = 1;
      If (match(HYPcontainer(HYPweightLx(?a),HYPweightRegL(HYPn1?,?b))) > 0) Redefine iLetter "0";
      .sort:Letter step in bootstrap;
    #EndDo
    If (count(HYPregL,1) > 0) Redefine iBoot "0";
    .sort:Boot step in bootstrap;
  #EndDo
#endprocedure
#procedure HypZeroLimitDistinctRegLimitHlogs(Var,LimitHlogFuncID)
  ClearTable HYPzeroLimitHlogTbl;
  #Define HYPtempZeroLimitExpr ""
  #Define HYPtempZeroLimitHiddens ""
  #call HypDistinctFuncsToExpression(HYPtempZeroLimitExpr,`LimitHlogFuncID',HYPwrap,HYPzeroLimitIndex,HYPaux,HYPn,`HYPtempPrefix',HYPtempCount,HYPtempZeroLimitHiddens,)
  #call HypSubstituteExtraSymbolZeroLimits(HYPzeroLimitSubstTbl,zerolimitsubst.h,`LimitHlogFuncID',HYPlim,HYPn)
  #call HypZeroLimitRegLimitHlogs(`LimitHlogFuncID',HYPLinfRegInfZero,HYPtoLinfRegInfZero,HYPtoLlimZero,`Var',HYPlead,HYPaux,HYPn)
  #call HypStoreExpressionInTableWithTag(`HYPtempZeroLimitExpr',HYPzeroLimitIndex,HYPzeroLimitHlogTbl,1)
  UnHide `HYPtempZeroLimitHiddens';
  Multiply Replace_(HYPzeroLimitIndex,HYPzeroLimitHlogTbl);
#endprocedure
#procedure HypBootstrapDistinctRegLimitHlogs(Var,VarDepExtrasExprID)
  .sort:Turning off PolyRatFun;
  PolyRatFun;
  #call HypTagVarDepFuncsThruExtras(`Var',HYPLinfRegInfZero,HYPregL,HYPwrap,HYPaux,`VarDepExtrasExprID')
  #call HypFillExtraSymbolZeroLimitTable(`Var',`VarDepExtrasExprID',HYPlim,HYPeval,HYPmark,HYPtag,HYPn,HYPvarDepTbl,HYPzeroLimitSubstTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,`HYPtempPrefix',HYPtempCount,zerolimitdump.txt,zerolimitsubst.h)
  #Define HYPtempRegLexpr ""
  #Define HYPtempHiddenActives ""
  #call HypDistinctFuncsToExpression(HYPtempRegLexpr,HYPregL,HYPwrap,HYPindex,HYPaux,HYPn,`HYPtempPrefix',HYPtempCount,HYPtempHiddenActives,)
  #Do iBoot=1,1
    id,once HYPregL(?a) = HYPcontainer(HYPweightLx,HYPweightRegL(?a));
    #Do iLetter=1,1
      #call HypDiffLogStepInBootStrap(HYPcontainer,HYPLx,HYPLlimZero,HYPweightLx,HYPweightRegL,HYPlogDiff,HYPaux,HYPn)
      .sort:Bracketing for zero limit;
      #call HypZeroLimitDistinctRegLimitHlogs(`Var',HYPLlimZero)
      .sort:Zero limits applied;
      #call HypDiffLogsToLetters(`Var',HYPcontainer,HYPweightLx,HYPweightRegL,HYPlogDiff,HYPpoly,HYPeval,HYPmark,,HYPfact,HYPwrap,HYPtag,HYPvars,HYPaux,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPsubstTbl,`HYPtempPrefix',HYPtempCount,HYPn)
      id HYPcontainer(HYPweightLx(?a),HYPweightRegL) = HYPLx(?a);
      id HYPLx = 1;
      If (match(HYPcontainer(HYPweightLx(?a),HYPweightRegL(HYPn1?,?b))) > 0) Redefine iLetter "0";
      .sort:Letter step in bootstrap;
    #EndDo
    If (count(HYPregL,1) > 0) Redefine iBoot "0";
    .sort:Boot step in bootstrap;
  #EndDo
  ClearTable HYPregHyperlogTbl;
  #call HypStoreExpressionInTableWithTag(`HYPtempRegLexpr',HYPindex,HYPregHyperlogTbl,1)
  .sort
  PolyRatFun HYPratep;
  Unhide `HYPtempHiddenActives';
  Bracket HYPindex;
  .sort:Bracketed HYPindex;
  Keep Brackets;
  Multiply Replace_(HYPindex,HYPregHyperlogTbl);
#endprocedure
#procedure HypDropTriviallyZeroRegularizedHyperLogs(RegLfuncID,AuxFuncID,SymbolID)
  If (count(`AuxFuncID',1) > 0);
    Print "`AuxFuncID' cannot be present in the routine...";
    Print "TERM: %t";
    Print "HypDropTriviallyZeroRegularizedHyperLogs";
    Exit;
  EndIf;
  If (match(`RegLfuncID'(?a,`SymbolID'1?!{`SymbolID'2?},?b,`SymbolID'2?!{`SymbolID'1?},?c)) == 0);
    id `RegLfuncID'(`SymbolID'1?{0,-1},?a) = 0;
  ElseIf (count(`RegLfuncID',1) > 1);
    Multiply Replace_(`RegLfuncID',`AuxFuncID');
    id `AuxFuncID'(?a,`SymbolID'1?!{`SymbolID'2?},?b,`SymbolID'2?!{`SymbolID'1?},?c) =
      `RegLfuncID'(?a,`SymbolID'1,?b,`SymbolID'2,?c)
    ;
    id `AuxFuncID'(`SymbolID'1?{0,-1},?a) = 0;
    Multiply Replace_(`AuxFuncID',`RegLfuncID');
  EndIf;
#endprocedure
#procedure HypShuffleRegularizeAtZeroStep(RegLfuncID,AuxFuncID,SymbolID,dFlag)
  If (count(`AuxFuncID'1,1,`AuxFuncID'2,1) > 0);
    Print "`AuxFuncID'1, `AuxFuncID'2 cannot be present in the routine...";
    Print "TERM: %t";
    Print "HypShuffleRegularizeAtZeroStep";
    Exit;
  EndIf;
  $`dFlag' = 0;
  ModuleOption,Local,$`dFlag';
  id,once,ifnomatch->ZeroRegLabel `RegLfuncID'(?a,0) = `AuxFuncID'1(?a)*`AuxFuncID'2(0);
  Repeat id `AuxFuncID'1(?a,0)*`AuxFuncID'2(?b) = `AuxFuncID'1(?a)*`AuxFuncID'2(0,?b);
  id `AuxFuncID'1(?a,`SymbolID'1?!{0,})*`AuxFuncID'2(?b) = 
    (-1)^nargs_(?b)*`AuxFuncID'1(?a)*`AuxFuncID'1(?b)*`AuxFuncID'2(`SymbolID'1)
  ;
  id `AuxFuncID'1 = 1;
  Shuffle `AuxFuncID'1;
  id `AuxFuncID'1(?a)*`AuxFuncID'2(`SymbolID'1?) = `RegLfuncID'(?a,`SymbolID'1);
  $`dFlag' = 1;
  Label ZeroRegLabel;
#endprocedure
#procedure HypShuffleRegularizeAtInfinityStep(RegLfuncID,LinfFuncID,AuxFuncID,SymbolID,dFlag)
  If (count(`AuxFuncID'1,1,`AuxFuncID'2,1,`AuxFuncID'3,1) > 0);
    Print "`AuxFuncID'1, `AuxFuncID'2, `AuxFuncID'3 cannot be present in the routine...";
    Print "TERM: %t";
    Print "HypShuffleRegularizeAtInfinityStep";
    Exit;
  EndIf;
  $`dFlag' = 0;
  ModuleOption,Local,$`dFlag';
  id,once,ifnomatch->InfinityRegLabel `RegLfuncID'(?a) = `AuxFuncID'1(?a);
  id,all `AuxFuncID'1(?a,`SymbolID'1?!{-1,},?b) = 
    (-1)^nargs_(?a)*(`AuxFuncID'1(`SymbolID'1) - `AuxFuncID'1(-1))*
    `AuxFuncID'2(nargs_(?a)-1,-1)*`AuxFuncID'3(?b)
  ;
  Repeat id `AuxFuncID'2(`SymbolID'1?{> 0},?a) = `AuxFuncID'2(`SymbolID'1 - 1,-1,?a);
  id `AuxFuncID'2(-1,-1) = 1;
  id `AuxFuncID'2(0,?a) = `AuxFuncID'3(?a);
  id `AuxFuncID'3 = 1;
  Shuffle `AuxFuncID'3;
  id `AuxFuncID'1(`SymbolID'1?)*`AuxFuncID'3(?a) = `LinfFuncID'(`SymbolID'1,?a);
  id `AuxFuncID'1 = `LinfFuncID'();
  $`dFlag' = 1;
  Label InfinityRegLabel;
#endprocedure
#procedure HypFromLinfToLoneForAlphabetZeroMinusOne(LinfFuncID,LoneFuncID,SplitFuncID,MinusFuncID,SymbolID)
  If (match(`LinfFuncID'(?a,`SymbolID'1?!{0,-1},?b)) > 0);
    Print "`LinfFuncID' is present with alphabet outside {0,-1}...";
    Print "TERM: %t";
    Print "HypFromLinfToLoneForAlphabetZeroMinusOne";
    Exit;
  EndIf;
  Transform,`LinfFuncID',replace(1,last)=(0,`SplitFuncID'(0,1),-1,`MinusFuncID'(1));
  Repeat;
    id `LinfFuncID'(?a,`SplitFuncID'(`SymbolID'1?,`SymbolID'2?),?b) =
      `LinfFuncID'(?a,`SymbolID'1,?b)
      -
      `LinfFuncID'(?a,`SymbolID'2,?b)
    ;
    id `LinfFuncID'(?a,`MinusFuncID'(`SymbolID'1?),?b) =
      - `LinfFuncID'(?a,`SymbolID'1,?b)
    ;
  EndRepeat;
  If (match(`LinfFuncID'(?a,`SymbolID'1?!{0,1},?b)) > 0);
    Print "Moebius produces alien letter...";
    Print "TERM: %t";
    Print "HypFromLinfToLoneForAlphabetZeroMinusOne";
    Exit;
  EndIf;
  id `LinfFuncID'(?a) = `LoneFuncID'(?a);
#endprocedure
#procedure HypFillTablesWithFunctionRange(ExprID,First,Last,FuncID,TableID)
  #If ("`ActiveExprNames_'" != "`ExprID'")
    #Write "More active expressions than desired..."
    #Write "We only want: `ExprID' we got: `ActiveExprNames_'"
    #Write "HypFillTablesWithFunctionRange"
    #Terminate
  #EndIf
  #Do iStage=`First',`Last'
    Bracket `FuncID'`iStage';
    .sort:Bracketed `FuncID'`iStage';
    FillExpression `TableID'`iStage' = `ExprID'(`FuncID'`iStage');
    .sort:Stage `iStage' stored;
  #EndDo
#endprocedure
#procedure HypDumpTablesToFile(FileName,FirstIndex,LastIndex,TableID)
  #If (`FirstIndex' > `LastIndex')
    #Write "Last index should be greater or equal to first index..."
    #Write "FirstIndex: `FirstIndex', LastIndex: `LastIndex'"
    #Write "HypDumpTablesToFile"
    #Terminate
  #ElseIf (`FirstIndex' < 1)
    #Write "FirstIndex can only be one or higher..."
    #Write "FirstIndex: `FirstIndex'"
    #Write "HypDumpTablesToFile"
    #Terminate
  #EndIf
  .sort:Dump to `FileName';
  #Create<`FileName'>
  #Write<`FileName'> ".sort:Loading tables;"
  #Do iIndex=`FirstIndex',`LastIndex'
    #If (`iIndex' == `FirstIndex')
      PrintTable `TableID'`iIndex' >> `FileName';
    #Else
      PrintTable `TableID'`iIndex' >> `FileName';
    #EndIf
  #EndDo
#endprocedure
#procedure HypTabulateRegLToMZV(FirstWeight,LastWeight,DropTempExpr)
  #If (`LastWeight' < `FirstWeight')
    #Write "Last weight should be greater or equal to first weight..."
    #Write "FirstWeight: `FirstWeight', LastWeight: `LastWeight'"
    #Write "HypTabulateRegLToMZV"
    #Terminate
  #ElseIf (`FirstWeight' < 1)
    #Write "FirstWeight can only be one or higher..."
    #Write "FirstWeight: `FirstWeight'"
    #Write "HypTabulateRegLToMZV"
    #Terminate
  #EndIf
  #call HypHideActiveExpressions()
  .sort:Actives are hidden;
  #call HypIncrementPreprocCounter(HYPtempCount)
  #Define MZVexpr "`HYPtempPrefix'`HYPtempCount'"
  local `MZVexpr' =
    #Do iWeight=`FirstWeight',`LastWeight'
      + 1
      #Do iSlot=1,`iWeight'
        *HYPslot(`iSlot')
      #EndDo
    #EndDo
  ;
  .sort:Init construction done;
  Repeat id HYPslot(HYPn1?number_) = HYPslot(HYPn1,0) + HYPslot(HYPn1,-1);
  .sort:(0,-1) gens complete;
  Multiply HYPregL;
  #Do iSlot=1,`LastWeight'
    id HYPregL(?a)*HYPslot(`iSlot',HYPn1?) = HYPregL(?a,HYPn1);
  #EndDo
  #call HypDropTriviallyZeroRegularizedHyperLogs(HYPregL,HYPaux,HYPn)
  id HYPregL(?a) = HYPregL(?a)*HYPwRegL(?a);
  #call HypShuffleRegularizeAtZeroStep(HYPregL,HYPaux,HYPn,ShuffleFlagZero)
  .sort:Reg at zero;
  #call HypShuffleRegularizeAtInfinityStep(HYPregL,HYPLinf,HYPaux,HYPn,ShuffleFlagInf)
  .sort:Reg at infinity;
  #call HypFromLinfToLoneForAlphabetZeroMinusOne(HYPLinf,HYPLone,HYPsplit,HYPminus,HYPn)
  id HYPLone = 1;
  .sort:Linf to Lone;
  #call HypFromLoneToHarmonic(`MZVexpr',HYPLone,HYPH,HYPaux,HYPaux2,HYPaux3,HYPn1,HYPn2)
  #call HypFromHarmonicToMZV(`MZVexpr',HYPH,mzv,HYPn,`LastWeight')
  .sort:MZVs appear;
  #Do ZetaVal={2,3,5,7,9,11,13}
    #If (`LastWeight' >= `ZetaVal')
      id z`ZetaVal' = HYPzeta`ZetaVal';
    #EndIf
  #EndDo
  #If (`LastWeight' >= 8)
    id z5z3 = HYPmzv5z3;
  #EndIf
  #If (`LastWeight' >= 10)
    id z7z3 = HYPmzv7z3;
  #EndIf
  #If (`LastWeight' >= 11)
    id z5z3z3 = HYPmzv5z3z3;
  #EndIf
  #If (`LastWeight' >= 12)
    id z9z3 = HYPmzv9z3;
    id z6z4z1z1 = HYPmzv6z4z1z1;
  #EndIf
  #If (`LastWeight' >= 13)
    id z7z3z3 = HYPmzv7z3z3;
    id z5z5z3 = HYPmzv5z5z3;
  #EndIf
  id HYPwRegL(?a) = HYPwRegL(?a)*HYPslot(nargs_(?a));
  #Do iWeight=`FirstWeight',`LastWeight'
    id HYPslot(`iWeight')*HYPwRegL(?a) = HYPwRegL`iWeight'(?a);
  #EndDo
  .sort:Weight funcs assigned;
  #call HypFillTablesWithFunctionRange(`MZVexpr',`FirstWeight',`LastWeight',HYPwRegL,HYPregLweightTbl)
  #call HypDumpTablesToFile(`HYPregLtoMZVtables',`FirstWeight',`LastWeight',HYPregLweightTbl)
  .sort:Tables dumped;
  #call HypUnhideActiveExpressions
  #If (`DropTempExpr' == 1)
    Drop `MZVexpr';
    .sort:`MZVexpr' purged;
  #EndIf
#endprocedure
#procedure HypIntegrationPreStep(Var)
  #call HypCreateVarSpectators(`HYPupperSpecName',`HYPupperSpecFile',`HYPlowerSpecName',`HYPlowerSpecFile',`HYPdoneSpecName',`HYPdoneSpecFile')
  .sort:Spectators created;
  #If (`HYPuseNormal' == 1)
    #call HypNormal()
  #Else
    #call HypSimpleSchwartzZippel(HYPnumz,HYPdenz,HYPlog,HYPLx,HYPLinfRegInfZero)
  #EndIf
  #call HypSymbolsToNumsDens(HYPnum,HYPden,HYPn,HYPratFunExceptions)
  .sort:After normal or S-Z;
  #Define ExtraSymbolExpr ""
  #call HypCreateVarDepExtrasymbolExpr(`Var',ExtraSymbolExpr)
  .sort:`ExtraSymbolExpr' created;
  #call HypRevealVarAsCoeffs(`Var')
  #call HypVarLogsToHyperlogs(HYPlogz,HYPlog,HYPLx,HYPn)
  .sort:Logs to Hlogs;
  #call HypCancelTowers(`Var')
  .sort:Towers canceled;
  #call HypPartialFractionAll(`Var',`HYPuseParking')
  .sort:PartFrac over;
  #If (`HYPuseNormal' == 0)
    #$HYPtempFoundQuadratics = 0;
    #$HYPtempRemnantQuadratics = 0;
    If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
      $HYPtempFoundQuadratics = 1;
      #call HypSetPolyFromCoeffs(HYPnumz,HYPaux,`Var',HYPn1,HYPn2,)
      Repeat;
        id HYPnumz(HYPn1?)*HYPnumz(HYPn2?) = HYPnumz(mul_(HYPn1,HYPn2));
      EndRepeat;
      #call HypGetPolyCoefficients(HYPnumz,`Var',HYPn1)
      Transform,HYPnumz,Replace(1,last)=(xarg_,HYPeval(xarg_));
    EndIf;
    ModuleOption,Maximum,$HYPtempFoundQuadratics;
    .sort:Checking for quadratics;
    #If (`$HYPtempFoundQuadratics' == 1)
      #Define HYPnumzcase "HYPnumz"
      #call HypCompressCoeffs(HYPeval,HYPmark,HYPvarDepTbl,HYPvarDepVars,HYPnumExtraSymbolLetters,HYPn,HYPratep,`HYPtempPrefix',HYPtempCount,HYPnumzcase)
      .sort:Quadratics compress;
      #call HypCancelTowers(`Var')
      If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
        Repeat;
          id HYPnumz(HYPn1?,HYPn2?,?a) = HYPnum(HYPn1) + `Var'*HYPnumz(HYPn2,?a);
          id HYPnumz(HYPn1?) = HYPnum(HYPn1);
        EndRepeat;
      EndIf;
      .sort:Quadratics cancel towers;
      #call HypSimpleSchwartzZippel(HYPnumz,HYPdenz,HYPlog,HYPLx,HYPLinfRegInfZero)
      If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
        $HYPtempRemnantQuadratics = 1;
        ModuleOption,Maximum,$HYPtempRemnantQuadratics;
      EndIf;
    #EndIf
  #EndIf
  #call HypPartialIntegrateAll(`Var',HYPinf,HYPzero)
  #call HypIncrementPreprocCounter(HYPtempCount)
  #Define HYPtempUpperExpr "`HYPtempPrefix'`HYPtempCount'"
  #call HypIncrementPreprocCounter(HYPtempCount)
  #Define HYPtempLowerExpr "`HYPtempPrefix'`HYPtempCount'"
  CopySpectator `HYPtempUpperExpr' = `HYPupperSpecName';
  CopySpectator `HYPtempLowerExpr' = `HYPlowerSpecName';
  .sort:Temp exprs from specs;
  EmptySpectator `HYPupperSpecName';
  EmptySpectator `HYPlowerSpecName';
  .sort:Specs emptied;
  ClearTable HYPzeroLimitSubstTbl;
  #call HypBootstrapDistinctRegLimitHlogs(`Var',`ExtraSymbolExpr')
  .sort:Bootstrap done;
  If (expression(`HYPtempUpperExpr'));
    If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
      Print "Quadratics in upper limit expression...";
      Print "TERM: %t";
      Exit;
    EndIf;
  EndIf;
  If (expression(`HYPtempLowerExpr'));
    If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
      Print "Quadratics in lower limit expression...";
      Print "TERM: %t";
      Exit;
    EndIf;
  EndIf;
  #call HypSimplifyNumericHyperlogs(HYPLinfRegInfZero,HYPn)
  .sort:ToNumbers done;
  If (expression(`HYPtempUpperExpr'));
    ToSpectator `HYPupperSpecName';
  EndIf;
  .sort:Upper limits to spec;
  #call HypHideActiveExpressions(`HYPtempLowerExpr')
  #call HypIsZeroAtZeroLimit(`Var',HYPnumz,HYPdenz,HYPLx,HYPaux,HYPdegree,HYPn1,IsZero)
  If ($IsZero == 1) Discard;
  .sort:Trivial zero limits dropped;
  ToSpectator `HYPlowerSpecName';
  .sort:Lower limits to spec;
  #call HypUnhideActiveExpressions
  drop `HYPtempUpperExpr',`HYPtempLowerExpr';
  .sort:temp limit exprs dropped;
  #If (`HYPuseNormal' == 0)
    #If (`$HYPtempRemnantQuadratics' == 1)
      If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
        Shuffle HYPLx;
        Shuffle HYPLinfRegInfZero;
      EndIf;
      .sort:Shuffled before S-Z screening;
      #call HypSimpleSchwartzZippel(HYPnumz,HYPdenz,HYPlog,HYPLx,HYPLinfRegInfZero)
      If (match(HYPdenz(HYPn1?,HYPn2?,HYPn3?)) > 0);
        Print "Quadratics survive the remnants too...";
        Print "TERM: %t";
        Exit;
      EndIf;
    #EndIf
  #EndIf
#endprocedure
#procedure HypIntegrationStep(Var)
  shuffle HYPLx;
  .sort:Words shuffled;
  #call HypBuildPrimitive(`Var',HYPnumz,HYPdenz,HYPnum,HYPden,HYPLx,HYPlim,HYPaux,HYPn,HYPinf,HYPzero)
  #call HypDropVanishingLimitTerms(`Var',HYPnumz,HYPdenz,HYPLx,HYPaux,HYPdegree,HYPn1,HYPlim,HYPinf,HYPzero)
  #call HypStashLimitTerms(HYPlim,HYPnum,HYPden,HYPinf,HYPzero,HYPn,`HYPupperSpecName',`HYPlowerSpecName')
  .sort:Primitive built, limits routed;
  #Do expr={`ActiveExprNames_',}
    #If ("`expr'" != "")
      #If (TermsIn(`expr') > 0)
        #Write "After integration no term should be kept in the active expressions..."
        #Write "`expr': %E",`expr';
        #Terminate
      #EndIf
    #EndIf
  #EndDo
#endprocedure
#procedure HypIntegrationPostStep(Var,MainIntegrandExprID)
  #call HypResolveUpperLimitTerms(`Var',HYPnumz,HYPdenz,HYPnum,HYPden,HYPLx,HYPLinfRegInfZero,HYPPhi,HYPaux,HYPdegree,HYPn,HYPinf,HYPzero,`HYPupperSpecName',`HYPlowerSpecName',`HYPdoneSpecName',`HYPtempPrefix',HYPtempCount)
  .sort:Upper limits resolved;
  #call HypResolveLowerLimitTerms(`Var',HYPnumz,HYPdenz,HYPLx,HYPLinfRegInfZero,HYPden,HYPnum,HYPaux1,HYPmark,HYPaux,HYPn,HYPzero,HYPlog0,`HYPlowerSpecName',`HYPdoneSpecName',`HYPtempPrefix',HYPtempCount,HYPzeroLimitSubstTbl,zerolimitsubst.h,HYPlim)
  .sort:Lower limits resolved;
  drop `ActiveExprNames_';
  .sort:Spent integrands dropped;
  #call HypRetrieveDoneTerms(`MainIntegrandExprID',`HYPdoneSpecName')
  .sort:Done terms retrieved;
  #call HypSimplifyNumericHyperlogs(HYPLinfRegInfZero,HYPn)
  .sort:Numeric Hlogs simplified;
  If ((match(HYPlogz(?a)) > 0) || (match(HYPLx(?a)) > 0));
    Print "Unresolved variable-dependent (hyper)log survived the integration of `Var'...";
    Print "TERM: %t";
    Print "HypIntegrationPostStep";
    Exit;
  EndIf;
  If (occurs(`Var') > 0);
    Print "`Var' still occurs after it was integrated out...";
    Print "TERM: %t";
    Print "HypIntegrationPostStep";
    Exit;
  EndIf;
  .sort:Postconditions checked;
  #call HypRemoveVarSpectators(`HYPupperSpecName',`HYPlowerSpecName',`HYPdoneSpecName',`HYPtempPrefix',HYPtempCount)
  .sort:Spectators removed;
  #call HypCompactAvatars(`Var',HYPratep)
#endprocedure
#procedure HypIntegrateVar(Var,ExprID)
  #call HypIntegrationPreStep(`Var')
  #call HypIntegrationStep(`Var')
  #call HypIntegrationPostStep(`Var',`ExprID')
#endprocedure
#procedure HypSimplifyNumericPowFuncs(PowFuncID,Sym1,...,Sym2)
  id `PowFuncID'(0,`Sym1'?) = 0;
  id `PowFuncID'(`Sym1'?,0) = 1;
  id `PowFuncID'(`Sym1'?number_,`Sym2'?number_) = `Sym1'^`Sym2';
#endprocedure
#procedure HypSimplifyNumericDenFuncs(DenFuncID,Sym1)
  id `DenFuncID'(`Sym1'?number_) = 1/`Sym1';
#endprocedure
#procedure HypSimplifyNumericNumFuncs(NumFuncID,Sym1)
  id `NumFuncID'(`Sym1'?number_) = `Sym1';
#endprocedure
#procedure HypSimplifyNumericRatFuncs(RatFuncID, Sym1, Sym2)
  id `RatFuncID'(`Sym1'?, `Sym2'?number_) = `Sym1'/`Sym2';
  id `RatFuncID'(0, `Sym1'?) = 0;
#endprocedure
#procedure HypSimplifyNumericLogFuncs(LogFuncID)
  id `LogFuncID'(1) = 0;
#endprocedure
#procedure HypSimplifyNumericVarDepLogFuncs(LogFuncID,Sym1,Sym2)
  id `LogFuncID'(`Sym1'?,`Sym2'?number_) = `LogFuncID'(`Sym1'/`Sym2');
#endprocedure
#procedure HypSimplifyNumericHyperlogWithEmptyArg(AuxFuncID,?HlogFuncIDs)
  id `AuxFuncID'?{`?HlogFuncIDs',} = 1;
#endprocedure
#procedure HypSimplifyHyperlogZeroLimit(HyperlogFuncID,Sym1)
  id `HyperlogFuncID'(?a,`Sym1'?!{0,}) = 0;
#endprocedure
#procedure HypVarSimplify(Var)
  id `Var'*HYPdenx(0) = 1;
  repeat id HYPlim(`Var',0)*HYPdenx(HYPn1?,HYPn2?) = HYPlim(`Var',0)*HYPnum(HYPn2)*HYPden(-HYPn1);
  repeat id HYPlim(`Var',0)*HYPdenx(HYPn1?!{0,}) = HYPlim(`Var',0)*HYPden(-HYPn1);
  id HYPlim(`Var',0)*`Var' = 0;
  if (match(HYPlim(`Var',HYPinf)) > 0);
    if (count(`Var',1) == 0);
      id HYPdenx(?a) = 0;
    endif;
  endif;
#endprocedure
#procedure HypSimplifyCompositeDenFuncs(DenFuncID,NumFuncID,RatFuncID,ArgNumFuncID,ArgDenFuncID,Sym1,Sym2)
  id `DenFuncID'(`RatFuncID'(`Sym1'?,`Sym2'?)) = `NumFuncID'(`Sym2')*`DenFuncID'(`Sym1');
  id `DenFuncID'(`ArgNumFuncID'(`Sym1'?)*`ArgDenFuncID'(`Sym2'?)) = `NumFuncID'(`Sym2')*`DenFuncID'(`Sym1');
#endprocedure
#procedure HypRecastDenominators()
  id HYPden(HYPden(HYPn1?)) = HYPn1;
  id HYPnum(HYPden(HYPn1?)) = HYPden(HYPn1);
  id HYPnum(HYPnum(HYPn1?)) = HYPnum(HYPn1);
  id HYPnum(HYPnum(HYPn1?)*HYPden(HYPn2?)) = HYPnum(HYPn1)*HYPden(HYPn2);
  id HYPden(HYPnum(HYPn1?)) = HYPden(HYPn1);
  splitarg((HYPden)),HYPden;
  id HYPden(HYPn1?,0) = HYPden(HYPn1);
  id HYPden(HYPn1?,HYPden(HYPn2?)) = HYPnum(HYPn2)*HYPden(HYPn1*HYPn2+1);
  id HYPden(HYPn1?,HYPden(HYPn2?)*HYPnum(HYPn3?)) = HYPnum(HYPn2)*HYPden(HYPn1*HYPn2+HYPn3);
  id HYPden(HYPn1?)*HYPnum(HYPn1?) = 1;
#endprocedure
#procedure HypPowFuncSimplify()
  id HYPden(HYPn1?)*HYPpow(HYPn1?,HYPn2?) = HYPpow(HYPn1,HYPn2-1);
  id HYPnum(HYPn1?)*HYPpow(HYPn1?,HYPn2?) = HYPpow(HYPn1,HYPn2+1);
  #call HypSimplifyPowerFuncs(HYPpow,HYPn1,...,HYPn3)
#endprocedure
#procedure HypDenSimplify(NumFuncID,DenFuncID,RatFuncID,IntVarID,Sym1,Sym2)
  #call HypSimplifyCompositeDenFuncs(`DenFuncID',`NumFuncID',`RatFuncID',`NumFuncID',`DenFuncID',`Sym1',`Sym2')
  id `DenFuncID'(`RatFuncID'(`Sym1'?,`Sym2'?)) = `NumFuncID'(`Sym2')*`DenFuncID'(`Sym1');
  id `NumFuncID'(`Sym1'?)*`DenFuncID'(`Sym1'?) = 1;
  #do i=1,`HYPNPARAMS'
    id `IntVarID'`i'*`DenFuncID'(`IntVarID'`i') = 1;
  #enddo
  #call HypSimplifyNumericDenFuncs(`DenFuncID',`Sym1')
#endprocedure
#procedure HypNumSimplify(NumeratorFuncID,Sym1)
  id `NumeratorFuncID'(`Sym1'?number_) = `Sym1';
#endprocedure
#procedure HypSimplify()
  id HYPpow(HYPn1?number_,HYPn2?number_) = HYPn1^HYPn2;
  factarg HYPpow,1;
  repeat id HYPpow(HYPn1?,HYPn2?,?a,HYPn3?) = 
    HYPpow(HYPn1,HYPn3)*HYPpow(HYPn2,?a,HYPn3);
  id HYPpow(HYPn1?number_,HYPn2?number_) = HYPn1^HYPn2;
  if (count(HYPpow,1) > 0);
    repeat id HYPn1?HYPparams = HYPpow(HYPn1,1);
  endif;
  repeat id HYPpow(HYPn1?HYPparams,HYPn2?)*HYPpow(HYPn1?HYPparams,HYPn3?) =
    HYPpow(HYPn1,HYPn2+HYPn3);
  repeat id HYPpow(HYPn1?,HYPn2?)*HYPpow(HYPn1?,HYPn3?) = HYPpow(HYPn1,HYPn2+HYPn3);
  id HYPpow(HYPn1?,0) = 1;
  id HYPpow(HYPn1?symbol_,HYPn2?) = HYPaux(HYPn1,HYPn2);
  id HYPaux(HYPn1?HYPparams,HYPn2?) = HYPpow(HYPn1,HYPn2);
  id HYPaux(HYPn1?,HYPn2?number_) = HYPn1^HYPn2;
  multiply replace_(HYPaux,HYPpow);
  #call HypDenSimplify(HYPnum,HYPden,HYPrat,HYPz,HYPn1,HYPn2)
  factarg HYPden;
  chainout HYPden;
  #call HypSimplifyNumericDenFuncs(HYPden,HYPn1)
  id HYPep*HYPden(HYPep) = 1;
  repeat id HYPden(HYPn1?)*HYPpow(HYPn1?,HYPn2?) = HYPpow(HYPn1,HYPn2-1);
  repeat id HYPnum(HYPn1?)*HYPden(HYPn1?) = 1;
  factarg HYPnum;
  chainout HYPnum;
  #call HypSimplifyNumericNumFuncs(HYPnum,HYPn1)
  repeat id HYPnum(HYPn1?)*HYPpow(HYPn1?,HYPn2?) = HYPpow(HYPn1,HYPn2+1);
  id HYPnum(HYPn1?)*HYPden(HYPn1?) = 1;
  id HYPlog(1) = 0;
  argument HYPLlimZero,HYPLx;
    argument HYPden;
      factarg,(-1),HYPrat,1;
      repeat;
        id HYPrat(?a,HYPn1?number_,?b,HYPn2?) = HYPrat(?a,?b,HYPn2/HYPn1);
      endrepeat;
      transform,HYPrat,mulargs(1,last-1);
    endargument;
    id HYPden(HYPrat(HYPn1?,HYPn2?)) = HYPrat(HYPn2,HYPn1);
    id HYPrat(HYPn1?, HYPn2?number_) = HYPn1/HYPn2;
  endargument;
#endprocedure
#procedure HypSimplifyPowerFuncs(PowerFuncID,Sym1,...,Sym3)
  id `PowerFuncID'(`Sym1'?, 0) = 1;
  factarg `PowerFuncID',1;
  repeat id `PowerFuncID'(`Sym1'?,`Sym2'?,?a,`Sym3'?) = 
    `PowerFuncID'(`Sym1',`Sym3')*`PowerFuncID'(`Sym2',?a,`Sym3')
  ;
  repeat;
    id `PowerFuncID'(`Sym1'?,`Sym2'?)*`PowerFuncID'(`Sym1'?,`Sym3'?) =
      `PowerFuncID'(`Sym1',`Sym2' + `Sym3')
    ;
  endrepeat;
  id `PowerFuncID'(`Sym1'?number_,`Sym2'?number_) = `Sym1'^`Sym2';
#endprocedure
#procedure HypSimplifyRatFuncsGCD(RatFuncID, DivFunc1ID, DivFunc2ID, GCDFuncID, Sym1, Sym2, Sym3)
  id `RatFuncID'(`Sym1'?, `Sym2'?!number_) = 
    `RatFuncID'(
      `DivFunc1ID'(`Sym1'), 
      `DivFunc2ID'(`Sym2'), 
      `GCDFuncID'(gcd_(`Sym1', `Sym2'))
    )
  ;
  id `RatFuncID'(`DivFunc1ID'(`Sym1'?), `DivFunc2ID'(`Sym2'?), `GCDFuncID'(1)) = 
    `RatFuncID'(`Sym1', `Sym2')
  ;
  id `RatFuncID'(`DivFunc1ID'(`Sym1'?), `DivFunc2ID'(`Sym2'?), `GCDFuncID'(`Sym3'?)) = 
    `RatFuncID'(div_(`Sym1', `Sym3'), div_(`Sym2', `Sym3'))
  ;

#endprocedure
#procedure HypSimplifyRatFuncArguments(ExprID, FuncID, RatFuncID, SimpFuncID, DivFunc1ID, DivFunc2ID, GCDFuncID, SimpTableID, Sym1, Sym2, Sym3)
  .sort:SimplifyRatFuncArguments starts;
  skip;
  nskip `ExprID';
  #define PreviousExtraSymbols "`ExtraSymbols_'"
  argument `FuncID';
    ToPolynomial, OnlyFunctions, `RatFuncID';
  endargument;
  .sort:`RatFuncID' converted to extras;
  skip;
  #$NewSymbols = `EXTRASYMBOLS_' - `PreviousExtraSymbols';
  #call HypExtraSymbolsToExpression(`ExprID'simp, `PreviousExtraSymbols', `ExtraSymbols_', 1, `SimpFuncID',0,,0)
  skip;
  nskip `ExprID'simp;
  id `SimpFuncID'(`Sym1'?, `Sym2'?) = `SimpFuncID'(`Sym1', extrasymbol_(`Sym1'));
  argument `SimpFuncID', 2;
    #call HypSimplifyRatFuncsGCD(`RatFuncID', `DivFunc1ID', `DivFunc2ID', `GCDFuncID', <`Sym1'>,...,<`Sym3'>)
    #call HypSimplifyNumericRatFuncs(`RatFuncID', <`Sym1'>,...,<`Sym2'>)
  endargument;
  id `SimpFuncID'(`Sym1'?, `Sym2'?) = `SimpFuncID'(`Sym1')*`Sym2';
  .sort:Bracketing `ExprID'simp;
  skip;
  nskip `ExprID'simp;
  bracket `SimpFuncID';
  .sort:Bracketed `ExprID'simp;
  fillexpression `SimpTableID' = `ExprID'simp(`SimpFuncID');
  skip;
  .sort:Filled `SimpTableID';
  skip;
  drop `ExprID'simp;
  .sort:Dropped `ExprID'simp;
  skip;
  nskip `ExprID';
  #do i={`PreviousExtraSymbols'+1}, `EXTRASYMBOLS_'
    argument `FuncID';
      id Z`i'_ = `SimpFuncID'(`i');
    endargument;
  #enddo
  .sort:Extras to functions;
  multiply replace_(`SimpFuncID', `SimpTableID');
  .sort:Simplified ratfuns substed;
  ClearTable `SimpTableID';
  delete ExtraSymbols>`PreviousExtraSymbols';
  skip;
  .sort:Purged simplification table and extra symbols;
  skip;
  .sort:SimplifyRatFuncArguments fin;
#endprocedure
#procedure HypSimplifyRatFuncsTrivial(FuncID, OldRatFuncID, NewRatFuncID, PolyFuncID, Sym1, Sym2)
  .sort:SimplifyRatFuncsTrivial start;
  argument `FuncID';
    id `OldRatFuncID'(`PolyFuncID'(`Sym1'?), `PolyFuncID'(`Sym2'?)) = `OldRatFuncID'(`Sym1', `Sym2');
    #call HypSimplifyNumericRatFuncs(`OldRatFuncID',`Sym1',`Sym2')
    multiply replace_(`OldRatFuncID', `NewRatFuncID');
  endargument;
  .sort:SimplifyRatFuncsTrivial fin;
#endprocedure
#procedure HypToNumbers
  id HYPnum(HYPn1?number_) = HYPn1;
  id HYPden(HYPn1?number_) = 1/HYPn1;
  id HYPrat?{HYPrat,HYPcrat}(HYPn1?,HYPn2?number_) = HYPn1/HYPn2;
  id HYPdenx(HYPn1?,HYPn2?number_) = HYPdenx(HYPn1/HYPn2);
  argument;
    id HYPrat?{HYPrat,HYPcrat}(HYPn1?,HYPn2?number_) = HYPn1/HYPn2;
    argument;
      id HYPrat?{HYPrat,HYPcrat}(HYPn1?,HYPn2?number_) = HYPn1/HYPn2;
    endargument;
  endargument;
  id HYPLinfRegInfZero(HYPn1?{0,-1}) = 0;
  id HYPlog(1) = 0;
#endprocedure
#procedure HypPolySimplify(ExprID,PolyFuncID,FinFuncID,<AuxFunc1ID>,...,<AuxFunc4ID>,Var,n1,...,n10)
  .sort:PolySimplify start;
  #do iSimp=1,1
    id,once `PolyFuncID'(?a) = `AuxFunc1ID'(?a);
    repeat id `AuxFunc1ID'(?a,0) = `AuxFunc1ID'(?a);
    id `AuxFunc1ID'(`n1'?,`n2'?) = `AuxFunc4ID'(`n1',`n2');
    if (count(`AuxFunc1ID',1) > 0);
      id `AuxFunc1ID'(?a) = `AuxFunc1ID'(nargs_(?a),?a);
      repeat;
        id `AuxFunc1ID'(`n1'?,`n2'?,?a,`n3'?) =
          `AuxFunc1ID'(`n1'-1,`n2' + `n3'*`Var'^(`n1'-1),?a);
      endrepeat;
      transform,`AuxFunc1ID',dropargs(1,1);
      factarg `AuxFunc1ID';
      chainout `AuxFunc1ID';
      multiply replace_(`AuxFunc1ID',`AuxFunc4ID');
    endif;
    if (count(`PolyFuncID',1) > 0) redefine iSimp "0";
    .sort:PolySimp loop over;
  #enddo
  #call HypGetPolyCoeffs(`AuxFunc4ID',`n1',`Var',1)
  id `AuxFunc4ID'(`n1'?) = `AuxFunc4ID'(`n1',0);
  multiply replace_(`AuxFunc4ID',`FinFuncID');
  .sort:PolySimplify fin;
#endprocedure
#procedure HypSimplifyVarDepDens(ExprID,OldDenFuncID,NewDenFunc,Sym1,...,Sym3)
  inexpression `ExprID';
    id `OldDenFuncID'(0,`Sym1'?!{0,}) = `NewDenFunc'(0);
    id `OldDenFuncID'(`Sym1'?,`Sym2'?) = `OldDenFuncID'(gcd_(`Sym1',`Sym2'),`Sym1',`Sym2');
    id `OldDenFuncID'(1,`Sym1'?,`Sym2'?) = `NewDenFunc'(`Sym1',`Sym2');
    id `OldDenFuncID'(<`Sym1'?>,...,<`Sym3'?>) = `NewDenFunc'(div_(`Sym2',`Sym1'),div_(`Sym3',`Sym1'));
  endinexpression;
#endprocedure
#procedure HypSimplifyDiffLogDiffFuncs(ExprID,DiffFuncID,RatFuncID,PolyFuncID,<AuxFunc1ID>,...,<AuxFunc4ID>,Var,al1,al2,be1,be2,ga1,ga2,de1,de2,Sym9,Sym10)
  .sort:SimplifyDiffLogDiffFuncs start;
  skip;
  nskip `ExprID';
  argument `DiffFuncID';
    argument `RatFuncID';
      id `PolyFuncID'(`al1'?) = `PolyFuncID'(`al1', 0);
    endargument;
  endargument;
  id `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1'?,`be1'?),`PolyFuncID'(`ga1'?,`de1'?)),
                  `RatFuncID'(`PolyFuncID'(`al2'?),`PolyFuncID'(`ga2'?))) 
    = `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1',`be1'),`PolyFuncID'(`ga1',`de1')),
                   `RatFuncID'(`PolyFuncID'(`al2', 0),`PolyFuncID'(`ga2', 0)));
  id `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1'?),`PolyFuncID'(`ga1'?)),
                  `RatFuncID'(`PolyFuncID'(`al2'?,`be2'?),`PolyFuncID'(`ga2'?,`de2'?)))
    = `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1', 0),`PolyFuncID'(`ga1', 0)),
                   `RatFuncID'(`PolyFuncID'(`al2',`be2'),`PolyFuncID'(`ga2',`de2')));
  id `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1'?),`PolyFuncID'(`ga1'?))) 
    = `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1',0),`PolyFuncID'(`ga1',0)));
  id `DiffFuncID'(0,`RatFuncID'(`PolyFuncID'(?a),`PolyFuncID'(?b))) = `DiffFuncID'(`RatFuncID'(`PolyFuncID'(?a), `PolyFuncID'(?b)));
  id `DiffFuncID'(`RatFuncID'(`PolyFuncID'(?a),`PolyFuncID'(?b)),0) = `DiffFuncID'(`RatFuncID'(`PolyFuncID'(?a), `PolyFuncID'(?b)));
  id `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1'?,`be1'?),`PolyFuncID'(`ga1'?,`de1'?)),
                  `RatFuncID'(`PolyFuncID'(`al2'?,`be2'?),`PolyFuncID'(`ga2'?,`de2'?)))
    = `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`RatFuncID'(`PolyFuncID'(`al1'?,`be1'?),`PolyFuncID'(`ga1'?,`de1'?)))
    = `DiffFuncID'(`al1',`be1') - `DiffFuncID'(`ga1',`de1');
  id `DiffFuncID'(`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,0,0) = 0;
  id `DiffFuncID'(`al1'?,`al2'?,0,`be2'?,`ga1'?,`ga2'?,0,0) = 
    `DiffFuncID'(`al2',`al1',`be2',0,`ga2',`ga1',0,0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1'),
                 gcd_(`ga1',`ga2'),
                 `al1',`al2',`be1',0,`ga1',`ga2',0,0);
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',0,0);
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),0,`ga1',`ga2',0,0);
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',0,`ga1',`ga2',0,0);
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1',`al2',`be1',0,div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),0,0);
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',0,0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1'*`ga2' - `al2'*`ga1', `be1'*`ga2');
  id `DiffFuncID'(`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,0,`de2'?) = 
    `DiffFuncID'(`al2',`al1',0,0,`ga2',`ga1',`de2',0);
  id `DiffFuncID'(`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) = 
    `DiffFuncID'(gcd_(`al1',`al2'),
                 gcd_(`ga1',`ga2',`de1'),
                 `al1',`al2',0,0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(1,1,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',0,0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),0,0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`Sym10',`al1',`al2',0,0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',0,0,div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),div_(`de1',`Sym10'),0);
  id `DiffFuncID'(1,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',0,0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al2'*`ga1'-`al1'*`ga2', `al2'*`de1') -
    `DiffFuncID'(`ga1', `de1');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1',`be2'),
                 gcd_(`ga1',`ga2'),
                 `al1',`al2',`be1',`be2',`ga1',`ga2',0,0);
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',0,0);
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),div_(`be2',`Sym9'),`ga1',`ga2',0,0);
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',`be2',`ga1',`ga2',0,0);
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),0,0);
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',0,0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,0) =
    `DiffFuncID'(`al1'*`ga2'-`al2'*`ga1', `be1'*`ga2'-`be2'*`ga1');
  id `DiffFuncID'(`al1'?,`al2'?,0,`be2'?,`ga1'?,`ga2'?,0,`de2'?) = 
    `DiffFuncID'(`al2',`al1',`be2',0,`ga2',`ga1',`de2',0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1'),
                 gcd_(`ga1',`ga2',`de1'),
                 `al1',`al2',`be1',0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',`be1',0,div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),div_(`de1',`Sym10'),0);
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1'*`ga2'-`al2'*`ga1', `be1'*`ga2'-`al2'*`de1') -
    `DiffFuncID'(`ga1', `de1');
  id `DiffFuncID'(`al1'?,`al2'?,0,`be2'?,`ga1'?,`ga2'?,`de1'?,0) = 
    `DiffFuncID'(`al2',`al1',`be2',0,`ga2',`ga1',0,`de1');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1'),
                 gcd_(`ga1',`ga2',`de2'),
                 `al1',`al2',`be1',0,`ga1',`ga2',0,`de2');
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',0,`de2');
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),0,`ga1',`ga2',0,`de2');
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',0,`ga1',`ga2',0,`de2');
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',0,div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),0,div_(`de2',`Sym10'));
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',0,`de2');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,0,`de2'?) =
    `DiffFuncID'(`PolyFuncID'(`al1'*`ga2'-`al2'*`ga1', `be1'*`ga2'+`al1'*`de2', `be1'*`de2')) - 
    `DiffFuncID'(`ga2', `de2');
  id `DiffFuncID'(`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) = 
    `DiffFuncID'(gcd_(`al1',`al2'),
                 gcd_(`ga1',`ga2',`de1',`de2'),
                 `al1',`al2',0,0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(1,1,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',0,0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),0,0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`Sym10',`al1',`al2',0,0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',0,0,div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),div_(`de1',`Sym10'),div_(`de2',`Sym10'));
  id `DiffFuncID'(1,`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',0,0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`al1'?,`al2'?,0,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1'*`ga2'-`al2'*`ga1', `al1'*`de2'-`al2'*`de1') -
    `DiffFuncID'(`ga1', `de1') -
    `DiffFuncID'(`ga2', `de2');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,0,`de2'?) = 
    `DiffFuncID'(`al2',`al1',`be2',`be1',`ga2',`ga1',`de2',0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1',`be2'),
                 gcd_(`ga1',`ga2',`de1'),
                 `al1',`al2',`be1',`be2',`ga1',`ga2',`de1',0);
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),div_(`be2',`Sym9'),`ga1',`ga2',`de1',0);
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),div_(`de1',`Sym10'),0);
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',0);
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,0) =
    `DiffFuncID'(`PolyFuncID'(`al2'*`ga1'-`al1'*`ga2', `be2'*`ga1'+`al2'*`de1'-`be1'*`ga2', `be2'*`de1')) -
    `DiffFuncID'(`ga1', `de1');
  id `DiffFuncID'(`al1'?,`al2'?,0,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) = 
    `DiffFuncID'(`al2',`al1',`be2',0,`ga2',`ga1',`de2',`de1');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1'),
                 gcd_(`ga1',`ga2',`de1',`de2'),
                 `al1',`al2',`be1',0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',0,div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),div_(`de1',`Sym10'),div_(`de2',`Sym10'));
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',0,`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,0,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`PolyFuncID'(`al1'*`ga2'-`al2'*`ga1', `be1'*`ga2'+`al1'*`de2'-`al2'*`de1', `be1'*`de2')) -
    `DiffFuncID'(`ga1', `de1') - `DiffFuncID'(`ga2', `de2');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) = 
    `DiffFuncID'(gcd_(`al1',`al2',`be1',`be2'),
                 gcd_(`ga1',`ga2',`de1',`de2'),
                 `al1',`al2',`be1',`be2',`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(1,1,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`Sym9'?!{1,},`Sym10'?,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`Sym10',div_(`al1',`Sym9'),div_(`al2',`Sym9'),div_(`be1',`Sym9'),div_(`be2',`Sym9'),
                         `ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(1,`Sym10'?,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`Sym10',`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`Sym10'?!{1,},`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',
                 div_(`ga1',`Sym10'),div_(`ga2',`Sym10'),div_(`de1',`Sym10'),div_(`de2',`Sym10'));
  id `DiffFuncID'(1,`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',`de2');
  id `DiffFuncID'(`al1'?,`al2'?,`be1'?,`be2'?,`ga1'?,`ga2'?,`de1'?,`de2'?) =
    `DiffFuncID'(`PolyFuncID'(`al1'*`ga2'-`al2'*`ga1',
                              `be1'*`ga2'-`be2'*`ga1'+`al1'*`de2'-`al2'*`de1', 
                              `be1'*`de2'-`be2'*`de1')) -
    `DiffFuncID'(`ga1', `de1') - `DiffFuncID'(`ga2', `de2');
  id `DiffFuncID'(`PolyFuncID'(`al1'?,0,0)) = 0;
  id `DiffFuncID'(`PolyFuncID'(`al1'?,`be1'?,`ga1'?)) =
    `DiffFuncID'(gcd_(`al1',`be1',`ga1'),
                 `PolyFuncID'(`al1',`be1',`ga1'));
  id `DiffFuncID'(1,`PolyFuncID'(`al1'?,`be1'?,`ga1'?)) =
    `DiffFuncID'(`PolyFuncID'(`al1',`be1',`ga1'));
  id `DiffFuncID'(`Sym9'?!{1,},`PolyFuncID'(`al1'?,`be1'?,`ga1'?)) =
    `DiffFuncID'(`PolyFuncID'(div_(`al1',`Sym9'),
                              div_(`be1',`Sym9'),
                              div_(`ga1',`Sym9')));
  id `DiffFuncID'(`PolyFuncID'(?a)) = `PolyFuncID'(?a);
  #call HypPolySimplify(`ExprID',`PolyFuncID',`PolyFuncID',`AuxFunc1ID',`AuxFunc2ID',`AuxFunc3ID',`AuxFunc4ID',`Var',`al1',`al2',`be1',`be2',`ga1',`ga2',`de1',`de2',`Sym9',`Sym10')
  #do iSplit=1,1
    id,once `PolyFuncID'(?a)*`PolyFuncID'(?b) = 
      `PolyFuncID'(?a) + `PolyFuncID'(?b);
    if (count(`PolyFuncID',1) > 1) redefine iSplit "0";
    .sort:Split polyproducts;
    skip;
    nskip `ExprID';
  #enddo
  id `PolyFuncID'(`al1'?,`be1'?) = `DiffFuncID'(`al1',`be1');
  id `PolyFuncID'(`al1'?,`be1'?,`ga1'?) = `DiffFuncID'(`PolyFuncID'(`al1',`be1',`ga1'));
  #if (`CheckPolysInSimplifyDiffLogDiffFuncs')
    if (match(`DiffFuncID'(`PolyFuncID'(`al1'?,`be1'?,`ga1'?))) > 0);
      print "Polynomial factor not being treated at the moment...";
      print "term: %t";
      print "Variable: `Var'";
      exit "SimplifyDiffLogDiffFuncs";
    endif;
  #endif
  id `DiffFuncID'(0,`be1'?!{0,}) = `DiffFuncID'(0,1);
  id `DiffFuncID'(`al1'?,0) = 0;
  id `DiffFuncID'(`al1'?!{0,},`be1'?!{0,}) = 
    `DiffFuncID'(gcd_(`al1',`be1'),`al1',`be1');
  id `DiffFuncID'(1,`al1'?,`be1'?) = `DiffFuncID'(`al1',`be1');
  id `DiffFuncID'(`Sym9'?,`al1'?,`be1'?) = 
    `DiffFuncID'(div_(`al1',`Sym9'),div_(`be1',`Sym9'));
#endprocedure
#procedure HypSimplifyGeneralDiffLogDiffFuncs(ExprID,DiffFuncID,RatFuncID,PolyFuncID,Sym1,...,Sym8)
  If ((match(`DiffFuncID'(`RatFuncID'(`PolyFuncID'(0,0),`PolyFuncID'(1,0)),?a)) ||
    match(`DiffFuncID'(`RatFuncID'(`PolyFuncID'(0),`PolyFuncID'(1)),?a))) > 0);
    Argument `DiffFuncID',2;
      Argument `RatFuncID',1;
        Argument `PolyFuncID';
          multiply -1;
        EndArgument;
      EndArgument;
    EndArgument;
  EndIf;
  id `DiffFuncID'(?a,`RatFuncID'(`PolyFuncID'(0,0),`PolyFuncID'(1,0)),?b) =
    `DiffFuncID'(?a,?b);
  also `DiffFuncID'(?a,`RatFuncID'(`PolyFuncID'(0),`PolyFuncID'(1)),?b) =
    `DiffFuncID'(?a,?b);
  Repeat;
    id `DiffFuncID'(
      `RatFuncID'(
        `PolyFuncID'(0,?a,`Sym1'?!{0,}), 
        `PolyFuncID'(?b, 0)
      )
      ) =
      `DiffFuncID'(0, 1)
      +
      `DiffFuncID'(`RatFuncID'(`PolyFuncID'(?a,`Sym1'),`PolyFuncID'(?b)))
    ;
    id `DiffFuncID'(
      `RatFuncID'(
        `PolyFuncID'(?a, 0),
        `PolyFuncID'(0,?b,`Sym1'?!{0,})
      )
      ) =
      -
      `DiffFuncID'(0, 1)
      +
      `DiffFuncID'(`RatFuncID'(`PolyFuncID'(?a),`PolyFuncID'(?b,`Sym1')))
    ;
    id `DiffFuncID'(
      `RatFuncID'(
        `PolyFuncID'(0,?a,`Sym1'?!{0,}), 
        `PolyFuncID'(?b, 0)
      ),
      `RatFuncID'(
        `PolyFuncID'(`Sym2'?!{0,},?c), 
        `PolyFuncID'(?d,0)
      )
      ) =
      `DiffFuncID'(0, 1)
      +
      `DiffFuncID'(
        `RatFuncID'(`PolyFuncID'(?a,`Sym1'),`PolyFuncID'(?b)),
        `RatFuncID'(`PolyFuncID'(`Sym2',?c),`PolyFuncID'(0,?d))
      )
    ;
  EndRepeat;
  If (
    match(
      `DiffFuncID'(
        `RatFuncID'(
          `PolyFuncID'(`Sym1'?$a,`Sym2'?$b,`Sym3'?!{0,}$c), 
          `PolyFuncID'(`Sym4'?,`Sym5'?,0)
        )
      )
    ) > 0
  );
    $CoeffRelation = $b - $a - $c;
    If ($CoeffRelation == 0);
      id `DiffFuncID'(
          `RatFuncID'(
            `PolyFuncID'($a,$b,$c), 
            `PolyFuncID'(`Sym1'?,`Sym2'?,0)
          )
        )
        =
        `DiffFuncID'(1,1)
        +
        `DiffFuncID'(
          `RatFuncID'(
            `PolyFuncID'($a,$c),
            `PolyFuncID'(1,0)
          )
        )
        -
        `DiffFuncID'(
          `RatFuncID'(
            `PolyFuncID'(`Sym1',`Sym2'),
            `PolyFuncID'(1,0)
          )
        )
      ;
    EndIf;
  EndIf;
  id `DiffFuncID'(
    `RatFuncID'(
      `PolyFuncID'(`Sym1'?!{0,},`Sym2'?,`Sym3'?!{0,}), 
      `PolyFuncID'(`Sym4'?!{0,},`Sym5'?!{0,},0)
    ),
    `RatFuncID'(
      `PolyFuncID'(`Sym6'?!{0,},`Sym7'?!{0,}), 
      `PolyFuncID'(1,0)
    )
    ) =
    `DiffFuncID'(
      `RatFuncID'(
        `PolyFuncID'(
          `Sym1' - `Sym4'*`Sym6',
          `Sym2' - `Sym4'*`Sym7' - `Sym5'*`Sym6',
          `Sym3' - `Sym5'*`Sym7'
        ), 
        `PolyFuncID'(`Sym4',`Sym5',0)
      )
    )
  ;
  #call HypDropTrailingZeroRatFuncCoeffs(`DiffFuncID',`RatFuncID',`PolyFuncID',`Sym1',...,`Sym2')
  #If (`CheckPolysInSimplifyDiffLogDiffFuncs')
    If (match(`DiffFuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1'?,`Sym2'?,`Sym3'?,?c),?d),?e)) > 0);
      print "At least quadratic polynom survives general simplification...";
      print "Affected term: %t";
      exit "HypSimplifyGeneralDiffLogDiffFuncs";
    EndIf;
  #EndIf
  ModuleOption,local,$a,$b,$c,$CoeffRelation;
  .sort:SimplifyGeneralDiffLogDiffFuncs;
#endprocedure
#procedure HypCheckForZeroCoeffs(ExprID,LinfFuncID,TempExprCountVar,CurrentVar,?Vars)
  .sort:Checking for zero coeffs;
  skip;
  nskip `ExprID';
  #call HypShuffleNonNumericHlogs(`LinfFuncID',HYPaux,HYPn1,0)
  #redefine `TempExprCountVar' "{``TempExprCountVar''+1}"
  #define TempExpr "Temp``TempExprCountVar''"
  #call HypCreateFunctionExpressionWithExtras(`TempExpr',HYPaux,1,`ExprID')
  #if (exists(`TempExpr') > 0) 
    skip;
    nskip `ExprID';
    bracket HYPaux;
    .sort:`ExprID' bracketed;
    keep brackets;
    hide;
    .sort
    #redefine `TempExprCountVar' "{``TempExprCountVar''+1}"
    #define ZeroFuncExpr "Temp``TempExprCountVar''"
    local `ZeroFuncExpr' = 0;
    #do func=`TempExpr'
      #redefine `TempExprCountVar' "{``TempExprCountVar''+1}"
      #define TempFuncExpr "Temp``TempExprCountVar''"
      local `TempFuncExpr' = `ExprID'[`func'];
      .sort:Created `TempFuncExpr';
      #if (termsin(`TempFuncExpr') == 0)
        #write "`TempFuncExpr' should not be empty..."
        #write "Tried to collect coeffs for `func'"
        #terminate
      #endif
      PolyRatFun HYPratep;
      #define iprime "0"
      InExpression `TempFuncExpr';
      #do var={`?Vars'}
        #redefine iprime "{`iprime'+1}"
        id `var' = prime_(`iprime');
        argument;
          id `var' = prime_(`iprime');
          argument;
            id `var' = prime_(`iprime');
            argument;
              id `var' = prime_(`iprime');
            endargument;
          endargument;
        endargument;
        id HYPnum(HYPn1?number_) = HYPn1;
        id HYPden(HYPn1?number_) = 1/HYPn1;
        argument;
          id HYPrat(HYPn1?number_,HYPn2?number_) = HYPn1/HYPn2;
        endargument;
      #enddo
      id HYPdenx(HYPn1?) = HYPden(prime_(`iprime')-HYPn1);
      id HYPdenx(HYPn1?,HYPn2?) = HYPden(prime_(`iprime')-HYPn1/HYPn2);
      id HYPden(HYPn1?number_) = 1/HYPn1;
      shuffle HYPLinfRegInfZero;
      EndInExpression;
      .sort
      #if (termsin(`TempFuncExpr') == 0)
        local `ZeroFuncExpr' = `ZeroFuncExpr' + `func';
        .sort:`func' added to zeros;
      #endif
      drop `TempFuncExpr';
      .sort:`TempFuncExpr' dropped;
    #enddo
    PolyRatFun;
    InExpression `ZeroFuncExpr';
      id HYPratep(?a) = 1;
    EndInExpression;
    .sort
    unhide;
    drop `TempExpr';
    .sort:Unhidden originals;
    skip;
    nskip `ExprID';
    #define ZeroOut "0"
    #do zerofunc=`ZeroFuncExpr'
      #redefine ZeroOut "{`ZeroOut'+1}"
      id `zerofunc' = 0;
    #enddo
    multiply replace_(HYPaux,`LinfFuncID');
    #message Zeroed out `ZeroOut' hyperlogs;
    #call HypIncrementZeroedOutCounter(ZeroOut)
    .sort:`LinfFuncID's with zero coeff dropped;
    drop `ZeroFuncExpr';
    .sort Dropped `TempExpr';
  #endif
#endprocedure
#procedure HypSimplifyNumsDens(NumFuncID,DenFuncID,Sym)
  #do func={`NumFuncID',`DenFuncID'}
    factarg `func';
  #enddo
  #do func={`NumFuncID',`DenFuncID'}
    chainout `func';
  #enddo
  id `NumFuncID'(`Sym'?number_) = `Sym';
  #call HypSimplifyNumericDenFuncs(`DenFuncID',`Sym')
  id `NumFuncID'(`Sym'?)*`DenFuncID'(`Sym'?) = 1;
#endprocedure
#procedure HypSimplifyNumerics()
  #call HypSimplifyNumericPowFuncs(HYPpow,HYPn1,...,HYPn2)
  #call HypSimplifyNumericDenFuncs(HYPden,HYPn1)
  #call HypSimplifyNumericDenFuncs(HYPdenep,HYPn1)
  #call HypSimplifyNumericNumFuncs(HYPnum,HYPn1)
  #call HypSimplifyNumericNumFuncs(HYPnumep,HYPn1)
  #call HypSimplifyNumericLogFuncs(HYPlog)
  #call HypSimplifyNumericVarDepLogFuncs(HYPlogx,HYPn1,HYPn2)
  #call HypSimplifyNumericHyperlogWithEmptyArg(HYPaux,HYPLx,HYPLphi,HYPLinfRegInfZero)
  Argument HYPLinfRegInfZero;
    #call HypSimplifyNumericRatFuncs(HYPrat,HYPn1,HYPn2)
  EndArgument;
  #call HypSimplifyHyperlogZeroLimit(HYPLlimZero,HYPn1)
  #call HypSimplifyCompositeDenFuncs(HYPden,HYPnum,HYPrat,HYPnum,HYPden,HYPn1,HYPn2)
#endprocedure
#procedure HypGenerateJKsubsets()
  multiply distrib_(0,,HYPsetJK,HYPaux,<HYPz1>,...,<HYPz`HYPNPARAMS'>);
  id HYPaux?{HYPaux,HYPsetJK} = 0;
  id HYPaux(?a) = 1;
  id HYPsetJK(?a) = distrib_(0,,HYPsetJ,HYPsetK,?a);
#endprocedure
#procedure HypGenerateDegreeOfDivergenceExpr(ExprID)
  .sort:Deg of Div exprs;
  #if (exists(Degrees) == 0)
    local Degrees = 1;
    .sort
    hide;
    nhide Degrees;
    #call HypGenerateJKsubsets
    .sort:JKs done;
  #else
    hide;
    unhide Degrees;
    .sort:Degrees restored;
  #endif
  .sort:Degrees gen;
#endprocedure
#procedure HypFromPowFunc()
  id HYPpow(HYPn1?,HYPn2?{>0}) = HYPnum(HYPn1)^HYPn2;
  id HYPpow(HYPn1?,HYPn2?{<0}) = HYPden(HYPn1)^(-HYPn2);
  splitarg((HYPep)),HYPpow,2;
  id HYPpow(HYPn1?,0,HYPn2?) = HYPpow(HYPn1,HYPn2);
  id HYPpow(HYPn1?,HYPn2?{>0},HYPn3?) = HYPpow(HYPn1,HYPn3)*HYPnum(HYPn1)^HYPn2;
  id HYPpow(HYPn1?,HYPn2?{<0},HYPn3?) = HYPpow(HYPn1,HYPn3)*HYPden(HYPn1)^(-HYPn2);
#endprocedure
#procedure HypStripPowerFunctions(ExprID)
  .sort:Strip powfuncts;
  hide;
  local TMP = `ExprID';
  #call HypFromPowFunc
  .sort:Separate num,den,pow;
  local `ExprID'numden = TMP*replace_(HYPpow,HYPdrop);
  local `ExprID'powfunc = TMP*replace_(HYPden,HYPdrop,HYPnum,HYPdrop,HYPdenep,HYPdrop,HYPnumep,HYPdrop);
  .sort
  drop TMP;
  id HYPdrop(?a) = 1;
  .sort:Strippowfuncs done;
#endprocedure
#procedure HypDropTrivialJKsets(ExprID,?ExtraVars)
  .sort:DropTrivialJKsets;
  hide;
  unhide Degrees;
  multiply `ExprID'powfunc;
  bracket HYPsetJ,HYPpow;
  .sort:bracketed for J;
  keep brackets;
  #do zi=1,`HYPNPARAMS'
    if (match(HYPsetJ(?a,HYPz`zi',?b)) > 0);
      argument HYPpow,1;
        id HYPz`zi' = prime_(`zi')*HYPlam;
      endargument;
    endif;
  #enddo
  .sort:J sub over;
  bracket HYPsetK,HYPpow;
  .sort:bracketed for K;
  keep brackets;
  #do zi=1,`HYPNPARAMS'
    if (match(HYPsetK(?a,HYPz`zi',?b)) > 0);
      argument HYPpow,1;
        id HYPz`zi' = prime_(`zi')/HYPlam;
      endargument;
    endif;
  #enddo
  .sort:K sub over;
  #do zi=1,`HYPNPARAMS'
    argument HYPpow,1;
      id HYPz`zi' = prime_(`zi');
    endargument;
  #enddo
  #if ("`?ExtraVars'" != "")
    #define zi "`HYPNPARAMS'"
    #do var={`?ExtraVars'}
      #redefine zi "{`zi'+1}"
      id `var' = prime_(`zi');
      Argument HYPpow,1;
        id `var' = prime_(`zi');
      EndArgument;
    #enddo
  #endif
  .sort:FF subs over;
  factarg HYPpow,1;
  repeat;
    id HYPpow(?a,HYPlam,?b,HYPn1?) = HYPpow(?a,?b,HYPn1)*HYPomega(HYPn1);
    id HYPpow(?a,1/HYPlam,?b,HYPn1?) = HYPpow(?a,?b,HYPn1)*HYPomega(-HYPn1);
  endrepeat;
  id HYPpow(?a) = 1;
  chainin HYPomega;
  transform,HYPomega,addargs(1,last);
  .sort:Omegas collected;
  id HYPomega(HYPn1?number_) = 0;
  if (count(HYPomega,1) == 0) discard;
  .sort:DropTrivialJKsets done;
#endprocedure
#procedure HypGetDivs(ExprID,DegreesExprID,?ExtraVars)
  .sort GetDivs started;
  hide;
  nhide `DegreesExprID';
  #if ("`?ExtraVars'" != "")
    #$cnt = nargs_(`?ExtraVars');
    #define NumExtraVars "`$cnt'"
  #else
    #define NumExtraVars "0"
  #endif
  splitarg((HYPep)),HYPomega;
  id HYPomega(?a,HYPn1?number_,?b) = HYPomega(?a,?b);
  transform,HYPomega,addargs(1,last);
  multiply `ExprID'numden;
  .sort:Multiplied with `ExprID'numden;
  bracket HYPnumep,HYPdenep;
  .sort:Bracketed eps;
  keep brackets;
  argument HYPnumep;
    id HYPep = prime_({`HYPNPARAMS'+`NumExtraVars'+1});
  endargument;
  id HYPnumep(HYPn1?number_) = HYPn1;
  if (count(HYPnumep,1) > 0);
    print "Having non-pure epsilon numerators...";
    print "%t";
    exit "GetDivs";
  endif;
  argument HYPdenep;
    id HYPep = prime_({`HYPNPARAMS'+`NumExtraVars'+1});
  endargument;
  id HYPdenep(HYPn1?number_) = 1/HYPn1;
  if (count(HYPdenep,1) > 0);
    print "Having non-pure epsilon denominators...";
    print "%t";
    exit "GetDivs";
  endif;
  .sort:ep substituted;
  #do zi=1,`HYPNPARAMS'
    if (match(HYPsetJ(?a,HYPz`zi',?b)) > 0);
      argument HYPnum;
        id HYPz`zi' = prime_(`zi')*HYPlam;
      endargument;
      argument HYPden;
        id HYPz`zi' = prime_(`zi')*HYPlam;
      endargument;
    elseif (match(HYPsetK(?a,HYPz`zi',?b)) > 0);
      argument HYPnum;
        id HYPz`zi' = prime_(`zi')/HYPlam;
      endargument;
      argument HYPden;
        id HYPz`zi' = prime_(`zi')/HYPlam;
      endargument;
    endif;
    id HYPnum(HYPn1?number_) = HYPn1;
    id HYPden(HYPn1?number_) = 1/HYPn1;
    factarg,HYPnum;
    chainout HYPnum;
    id HYPnum(HYPlam) = HYPlam;
    id HYPnum(1/HYPlam) = 1/HYPlam;
    factarg,HYPden;
    chainout HYPden;
    id HYPden(HYPlam) = 1/HYPlam;
    id HYPden(1/HYPlam) = HYPlam;
    .sort:z`zi' done;
  #enddo
  #if ("`?ExtraVars'" != "")
    #define zi "`HYPNPARAMS'"
    #do var={`?ExtraVars'}
      #redefine zi "{`zi'+1}"
      id `var' = prime_(`zi');
      Argument HYPnum,HYPden;
        id `var' = prime_(`zi');
      EndArgument;
      id HYPnum(HYPn1?number_) = HYPn1;
      id HYPden(HYPn1?number_) = 1/HYPn1;
      factarg,HYPnum;
      chainout HYPnum;
      id HYPnum(HYPlam) = HYPlam;
      id HYPnum(1/HYPlam) = 1/HYPlam;
      factarg,HYPden;
      chainout HYPden;
      id HYPden(HYPlam) = 1/HYPlam;
      id HYPden(1/HYPlam) = HYPlam;
      .sort:`var' done;
    #enddo
  #endif
  #do zi=1,`HYPNPARAMS'
    id HYPz`zi' = prime_(`zi');
    argument HYPnum;
      id HYPz`zi' = prime_(`zi');
    endargument;
    argument HYPden;
      id HYPz`zi' = prime_(`zi');
    endargument;
    id HYPnum(HYPn1?number_) = HYPn1;
    id HYPden(HYPn1?number_) = 1/HYPn1;
    .sort:z`zi' done;
  #enddo
  id HYPsetJ(?a)*HYPsetK(?b) = 
    HYPsetJ(?a)*HYPsetK(?b)*
    HYPlam^(nargs_(?a) - nargs_(?b))*HYPord(0)
  ;
  #call HypLambdaExpand(HYPlam,HYPord,HYPden,HYPnum,HYPn1)
  .sort:Expanded in lambda;
  dropcoefficient;
  .sort:Coeffs gone;
  bracket HYPsetJ,HYPsetK,HYPomega;
  .sort:Bracketed;
  collect HYPacc;
  .sort:Collected;
  id HYPacc(HYPn1?number_) = 1;
  factarg HYPacc;
  repeat;
    id HYPacc(?a,1/HYPlam,?b) = HYPacc(?a,?b)/HYPlam;
    id HYPacc(?a,HYPlam,?b) = HYPacc(?a,?b)*HYPlam;
    id HYPacc(?a,HYPn1?number_,?b) = HYPacc(?a,?b);
    id HYPacc = 1;
  endrepeat;
  .sort
  argument HYPacc;
    multiply replace_(HYPlam,0);
  endargument;
  repeat;
    id HYPacc(?a,HYPn1?number_,?b) = HYPacc(?a,?b);
    id HYPacc = 1;
  endrepeat;
  .sort:HYPacc simplified;
  $cnt = count_(HYPlam,1);
  id HYPsetJ(?a)*HYPsetK(?b)*HYPomega(HYPn1?) = 
    HYPsetJ(?a)*HYPsetK(?b)*HYPomega(HYPn1+$cnt)
  ;
  multiply HYPlam^(-$cnt);
  splitarg,((HYPep)),HYPomega;
  id HYPomega(HYPn1?{>0,},?a) = 0;
  transform,HYPomega,addargs(first,last);
  ModuleOption,local,$cnt;
  .sort GetDivs done;
  #if 0
  #endif
#endprocedure
#procedure HypSelectDiv(ExprID,DegreesExprID)
  .sort:SelectDiv;
  hide;
  local Divs = `DegreesExprID';
  id HYPsetJ(?a)*HYPsetK(?b) = HYPsetJ(?a)*HYPsetK(?b)*
                               HYPtag(abs_(nargs_(?a) - nargs_(?b)));
  .sort:Bracket for tags;
  bracket HYPtag;
  .sort:Bracketed tags;
  hide Divs;
  #do i=`HYPNPARAMS',1,-1
    local SelDiv = Divs[HYPtag(`i')];
    .sort;
    #if (termsin(SelDiv) > 0)
      #breakdo
    #endif
  #enddo
  #$trm = firstterm_(SelDiv);
  #write "Config to regularize: %$",$trm;
  .sort
  local `ExprID' = `ExprID'*$trm;
  .sort:Div selected;
  if (expression(`ExprID'));
    id HYPsetJ(?a)*HYPsetK(?b)*HYPomega(HYPn1?) = 
      HYPsetJ(?a)*
      HYPsetK(?b)*
      HYPomega(HYPn1)*
      HYPdeg(HYPn1 - nargs_(?a) + nargs_(?b));
  endif;
  .sort:Degree added;
  unhide;
  drop Divs,SelDiv;
  .sort:End SelectDiv;
#endprocedure
#procedure HypDifferentiatePowerFuncs(PowerFuncID,DiffFuncID,ToDiffFuncID,DiffedFuncID,ExpFuncID,AuxFuncID,Sym1,...,Sym3,dVar,WithExtras)
  repeat;
    if (count(`DiffFuncID',1) > 0);
      id `DiffFuncID'(`Sym1'?$`dVar')*`PowerFuncID'(`Sym2'?,`Sym3'?) = 
        `ExpFuncID'(`Sym3')*`PowerFuncID'(`Sym2',`Sym3'-1)*`ToDiffFuncID'(`Sym2',1) +
        `DiffFuncID'(`Sym1')*`AuxFuncID'(`Sym2',`Sym3')
      ;
      Argument `ToDiffFuncID',1;
        #if (`WithExtras')
          FromPolynomial;
        #endif
        multiply count_($`dVar',1)/$`dVar';
      EndArgument;
      id `ToDiffFuncID'(`Sym1'?,`Sym2'?) = `DiffedFuncID'(`Sym1',`Sym2');
      if (count(`PowerFuncID',1) == 0) id `DiffFuncID'(`Sym1'?) = 0;
      #call HypSimplifyPowerFuncs(`PowerFuncID',`Sym1',`Sym2',`Sym3')
      #call HypNumSimplify(`ExpFuncID',`Sym1')
      #if ("`PowerFuncID'" != "`DiffedFuncID'")
        #call HypSimplifyPowerFuncs(`DiffedFuncID',`Sym1',`Sym2',`Sym3')
        #call HypNumSimplify(`ExpFuncID',`Sym1')
      #endif
    endif;
  endrepeat;
  multiply replace_(`AuxFuncID',`PowerFuncID');
  #if ("`PowerFuncID'" == "`DiffedFuncID'")
    #call HypSimplifyPowerFuncs(`PowerFuncID',`Sym1',`Sym2',`Sym3')
    #call HypNumSimplify(`ExpFuncID',`Sym1')
  #endif
#endprocedure
#procedure HypDifferentiateToRegularize()
  #do i=1,1
    #$it = 1;
    id HYPdeg(?a)*HYPsetJ*HYPsetK = 0;
    if (match(HYPsetJ(HYPn1?,?a)) > 0);
      id HYPdeg(?a)*HYPsetJ(HYPn1?,?b)*HYPsetK(?c) = 
        HYPdeg(?a)*HYPsetJ(?b)*HYPsetK(?c) - 
        HYPaux2(HYPn1,1)*HYPdiff(HYPn1)
      ;
    elseif (match(HYPsetK(HYPn1?,?a)) > 0);
      id HYPdeg(?a)*HYPsetJ(?b)*HYPsetK(HYPn1?,?c) = 
        HYPdeg(?a)*HYPsetJ(?b)*HYPsetK(?c) + 
        HYPaux2(HYPn1,1)*HYPdiff(HYPn1)
      ;
    endif;
    #call HypDifferentiatePowerFuncs(HYPpow,HYPdiff,HYPd,HYPaux2,HYPnumep,HYPaux,HYPn1,...,HYPn3,diffvar,1)
    repeat;
      id HYPaux2(HYPn1?, HYPn2?)*HYPaux2(HYPn3?, HYPn2?) = 
        HYPaux2(HYPn1*HYPn3, HYPn2)
      ;
    endrepeat;
    multiply replace_(HYPaux2,HYPpow);
    if (count(HYPdeg,1,HYPsetJ,1,HYPsetK,1) > 0) $it = 0;
    ModuleOption,local,$diffvar;
    ModuleOption,minimum,$it;
    .sort
    #redefine i "`$it'"
  #enddo
#endprocedure
#procedure HypRegularize(ExprID)
  .sort:Regularize;
  hide;
  nhide `ExprID';
  argtoextrasymbol HYPpow,1;
  .sort:Args to extrasymbols;
  id HYPomega(HYPn1?) = HYPdenep(HYPn1);
  id HYPdeg(HYPn1?)*HYPsetJ(?a)*HYPsetK(?b) = 
    HYPnumep(HYPn1) + HYPdeg(HYPn1)*HYPsetJ(?a)*HYPsetK(?b)
  ;
  if (count(HYPden,1,HYPnum,1) > 0);
    print "den or num found: %t";
    exit;
  endif;
  #call HypDifferentiateToRegularize()
  Argument HYPpow,1;
    FromPolynomial;
  EndArgument;
  id HYPnumep(HYPn1?) = HYPratep(HYPn1, 1);
  id HYPdenep(HYPn1?) = HYPratep(1, HYPn1);
  .sort:Before PolyRatFun;
  PolyRatFun HYPratep;
  .sort;After PolyRatFun;
  PolyRatFun;
  id HYPpow(HYPn1?, HYPn2?number_) = HYPaux(HYPn1, HYPn2);
  .sort
  abracket HYPpow,HYPrat;
  .sort
  keep brackets;
  id HYPaux(HYPn1?, HYPn2?) = HYPn1^HYPn2;
  .sort
  bracket HYPpow,HYPratep;
  .sort
  collect HYPacc;
  factarg HYPacc;
  repeat;
    id HYPacc(?a,HYPn1?number_,?b) = HYPn1*HYPacc(?a,?b);
    id HYPacc = 1;
  endrepeat;
  chainout HYPacc;
  id HYPacc(HYPn1?) = HYPpow(HYPn1, 1);
  #call HypSimplifyPowerFuncs(HYPpow,HYPn1,HYPn2,HYPn3)
  .sort
  PolyRatFun HYPratep;
  .sort
  PolyRatFun;
  .sort
  id HYPratep(HYPn1?, HYPn2?) = HYPnumep(HYPn1)*HYPdenep(HYPn2);
  id HYPnumep(HYPn1?number_) = HYPn1;
  id HYPdenep(HYPn1?number_) = 1/HYPn1;
  .sort:Dropping used extras;
  Delete,ExtraSymbols;
  .sort:Extras purged;
#endprocedure
#procedure HypAutoRegularize(ExprID,?ExtraVars)
  .sort:Reg. starts;
  #define ItCount "0"
  #do RegIt=1,1
    #if (`ItCount' == `MAXREGIT')
      #write "AutoRegularize reached the `ItCount'th iteration..."
      #write "Regularization reached maximal number of iterations..."
      #write "This can be a bug or a very deeply singular integrand..."
      #write "Change MAXREGIT or file an issue..."
      #terminate 1;
    #endif
    #redefine ItCount "{`ItCount'+1}"
    #call HypGenerateDegreeOfDivergenceExpr(`ExprID')
    #call HypStripPowerFunctions(`ExprID')
    #if (`ItCount' == 1)
      #call HypDropTrivialJKsets(`ExprID',`?ExtraVars')
    #else
      unhide Degrees;
      .sort:Degrees revealed;
    #endif
    #call HypGetDivs(`ExprID',Degrees,`?ExtraVars')
    #if (termsin(Degrees) > 0) 
      #redefine RegIt "0"
      #call HypSelectDiv(`ExprID',Degrees)
      hide `ExprID'numden,`ExprID'powfunc,Degrees;
      #call HypRegularize(`ExprID')
      #call HypSimplify
    #else
      drop Degrees,`ExprID'numden,`ExprID'powfunc;
    #endif
    .sort:`ItCount'th it. finished;
  #enddo
  unhide `ExprID';
  .sort:AutoRegularize done;
#endprocedure
#procedure HypShuffleRegularizeAtZero(ExprID,FuncID,RatFuncID,PolyFuncID,UseFilter,FilterFuncID,Sym1,Sym2,<AuxFunc1ID>,...,<AuxFunc3ID>)
  .sort:ShuffleRegularizeAtZero start;
  #do iReg=1,1
    skip;
    nskip `ExprID';
    if (
    #if (`UseFilter')
      count(`FilterFuncID',1) > 0
    #else
      1
    #endif
    );
      id `FuncID'(?a,`RatFuncID'(0, 1)) = `AuxFunc1ID'(?a,`RatFuncID'(0, 1));
      id `FuncID'(?a,0) = `AuxFunc1ID'(?a,0);
      id `FuncID'(?a,`RatFuncID'(`PolyFuncID'(0), `PolyFuncID'(1))) = `AuxFunc1ID'(?a,`RatFuncID'(`PolyFuncID'(0), `PolyFuncID'(1)));
      multiply replace_(`FuncID', `AuxFunc3ID');
      multiply replace_(`AuxFunc1ID', `FuncID');
      id,once `FuncID'(?a) = `AuxFunc1ID'(?a)*`AuxFunc2ID';
    endif;
    repeat;
      id `AuxFunc1ID'(?a,`RatFuncID'(0, 1))*`AuxFunc2ID'(?b) = `AuxFunc1ID'(?a)*`AuxFunc2ID'(?b,`RatFuncID'(0, 1));
      id `AuxFunc1ID'(?a, 0)*`AuxFunc2ID'(?b) = `AuxFunc1ID'(?a)*`AuxFunc2ID'(?b, 0);
      id `AuxFunc1ID'(?a,`RatFuncID'(`PolyFuncID'(0), `PolyFuncID'(1)))*`AuxFunc2ID'(?b) = `AuxFunc1ID'(?a)*`AuxFunc2ID'(?b,`RatFuncID'(`PolyFuncID'(0), `PolyFuncID'(1)));
    endrepeat;
    id `AuxFunc1ID'(?a)*`AuxFunc2ID'(?b) = 
      #if (`KeepLinfZeroInZeroRegularization')
      `AuxFunc3ID'(?a)*`AuxFunc3ID'(?b) 
      #endif
      - `AuxFunc2ID'(?b)*`AuxFunc2ID'(?a) + `AuxFunc2ID'(?a,?b);
    id `AuxFunc2ID' = 1;
    id `AuxFunc3ID' = 1;
    shuffle `AuxFunc2ID';
    multiply replace_(`AuxFunc2ID', `FuncID');
    if (
    #if (`UseFilter')
      count(`FilterFuncID',1) > 0
    #else
      1
    #endif
    );
      if (count(`FuncID',1) > 0) redefine iReg "0";
    endif;
    .sort:iReg loop at end;
  #enddo
  skip;
  nskip `ExprID';
  multiply replace_(`AuxFunc3ID',`FuncID');
#endprocedure
#procedure HypShuffleRegularizeAtInfinity(ExprID,FuncID,UseFilter,FilterFuncID,Sym1,Sym2,<Aux1FuncID>,...,<Aux4FuncID>)
  .sort:ShuffleRegularizeAtInfinity start;
  #do iReg=1,1
    #$iReg = 1;
    skip;
    nskip `ExprID';
    if (
    #if (`UseFilter')
      count(`FilterFuncID',1) > 0
    #else
      1
    #endif
    );
      id,once `FuncID'(?a) = `Aux1FuncID'(nargs_(?a),?a);
    endif;
    id `Aux1FuncID'(`Sym1'?number_,?a) = 
      sum_(
        `Sym2',
        1,
        `Sym1',
        (-1)^(`Sym2'-1)*
          (`Aux1FuncID'(`Sym2',?a) - `Aux1FuncID'(-1))*
          `Aux2FuncID'(-1)^(`Sym2'-1)*`Aux3FuncID'(`Sym2',?a)
      )
    ;
    chainin `Aux2FuncID';
    if (match(`Aux1FuncID'(`Sym1'?$n,`Sym2'?,?a)) > 0);
      transform,`Aux1FuncID',dropargs(1,$n);
      if (match(`Aux1FuncID'(`Sym1'?,`Sym2'?,?a)) > 0);
        transform,`Aux1FuncID',dropargs(2,last);
      endif;
    endif;
    if (count(`Aux3FuncID',1) > 0);
      id `Aux3FuncID'(`Sym1'?$n,?a) = `Aux3FuncID'(?a);
      transform,`Aux3FuncID',dropargs(1,$n);
    endif;
    multiply replace_(`Aux2FuncID',`Aux3FuncID');
    id `Aux3FuncID' = 1;
    shuffle `Aux3FuncID';
    id `Aux1FuncID'(?a)*`Aux3FuncID'(?b) = `Aux4FuncID'(?a,?b);
    multiply replace_(`Aux1FuncID',`Aux4FuncID');
    if (
    #if (`UseFilter')
      count(`FilterFuncID',1) > 0
    #else
      1
    #endif
    );
      if (count(`FuncID',1) > 0) $iReg = 0;
    endif;
    ModuleOption,local,$n;
    ModuleOption,minimum,$iReg;
    .sort:iReg loop at end;
    #redefine iReg "`$iReg'"
  #enddo
  skip;
  nskip `ExprID';
  multiply replace_(`Aux4FuncID',`FuncID');
#endprocedure
#procedure HypLambdaExpand(LambdaSymbol,OrdoFuncID,DenFuncID,NumFuncID,Sym1)
  .sort:LambdaExpand starts;
  id `OrdoFuncID'(`Sym1'?$ord) = `OrdoFuncID'($ord);
  if (count(`LambdaSymbol',1) > $ord) discard;
  factarg `DenFuncID';
  factarg `NumFuncID';
  chainout `DenFuncID';
  chainout `NumFuncID';
  id `DenFuncID'(`LambdaSymbol'^-1) = `LambdaSymbol';
  id `DenFuncID'(`LambdaSymbol') = 1/`LambdaSymbol';
  id `DenFuncID'(`Sym1'?number_) = 1/`Sym1';
  id `NumFuncID'(`Sym1'?) = `Sym1';
  normalize,^-1,`DenFuncID';
  splitarg,((`LambdaSymbol')),`DenFuncID';
  transform,`DenFuncID',addargs(2,last);
  Argument `DenFuncID',2;
    multiply 1/`LambdaSymbol';
  EndArgument;
  repeat;
    if (count(`LambdaSymbol',1) > $ord);
      discard;
    elseif (count(`LambdaSymbol',1) == $ord);
      id `DenFuncID'(?a) = 1;
    else;
      id,once `DenFuncID'(1,`Sym1'?) = 1 - `LambdaSymbol'*`Sym1'*`DenFuncID'(1,`Sym1');
    endif;
  endrepeat;
  id `OrdoFuncID'($ord) = 1;
  #if 0
  #endif
  ModuleOption,local,$ord;
  .sort:LambdaExpand fins;
#endprocedure
#procedure HypEpExpand
  .sort:Turning off PolyRatFun;
  PolyRatFun;
  .sort:No PolyRatFun;
  FactArg HYPdenep;
  ChainOut HYPdenep;
  Repeat;
    id HYPdenep(HYPn1?number_) = 1/HYPn1;
    id HYPdenep(HYPep) = 1/HYPep;
  EndRepeat;
  Repeat id HYPep = HYPnumep(HYPep);
  $PowerBudget = `HYPMAXEP' - count_(HYPep,1);
  ModuleOption,Local,$PowerBudget;
  If (count(HYPep,1) != 0);
    If (count(HYPratep,1) > 0);
      id HYPep^(-$PowerBudget+`HYPMAXEP')*HYPratep(HYPn1?,HYPn2?) = HYPratep(HYPn1,HYPep^(-`HYPMAXEP'+$PowerBudget)*HYPn2);
    Else;
      id HYPep^(-$PowerBudget+`HYPMAXEP') = HYPratep(1,HYPep^(-`HYPMAXEP'+$PowerBudget));
    EndIf;
  Else;
    If (count(HYPratep,1) == 0);
      Multiply HYPratep(1,1);
    EndIf;
  EndIf;
  Repeat;
    id,once HYPnumep(HYPn1?)*HYPratep(HYPn2?,HYPn3?) = HYPratep(HYPn1*HYPn2,HYPn3);
    Argument,HYPratep,1;
      If (count(HYPep,1) > $PowerBudget) Discard;
    EndArgument;
  EndRepeat;
  #call HypGetPolyCoefficients(HYPdenep,HYPep,HYPn1)
  id HYPdenep(?a) = HYPdenep(HYPaux(?a));
  Repeat;
    id HYPdenep(HYPaux(?a,HYPn1?),?b) = HYPdenep(HYPaux(?a),HYPn1*HYPep^nargs_(?a),?b);
  EndRepeat;
  id HYPdenep(HYPaux,?a) = HYPdenep(?a);
  Transform,HYPdenep,AddArgs(2,last);
  Repeat id HYPdenep(HYPn1?)*HYPratep(HYPn2?,HYPn3?) = HYPratep(HYPn2,HYPn1*HYPn3);
  Repeat id HYPdenep(HYPn1?number_,HYPn2?) = HYPe(HYPn2/HYPn1)/HYPn1;
  Repeat id HYPdenep(HYPn1?,HYPn2?)*HYPratep(HYPn3?,HYPn4?) = HYPe(HYPn2/HYPn1)*HYPratep(HYPn3,HYPn1*HYPn4);
  #do i=1,1
    id,once HYPe(HYPn1?)*HYPratep(HYPn2?,HYPn3?) = HYPratep(HYPn2,HYPn3) - HYPratep(HYPn1*HYPn2,HYPn3)*HYPe(HYPn1);
    Argument HYPratep,2;
      $PowerBudget = `HYPMAXEP' + count_(HYPep,1);
      ModuleOption,Local,$PowerBudget;
    EndArgument;
    Argument,HYPratep,1;
      If (count(HYPep,1) > $PowerBudget) Discard;
    EndArgument;
    id HYPratep(0,HYPn1?) = 0;
    if (count(HYPe,1) > 0) redefine i "0";
    .sort:e func expand;
  #enddo
  Repeat;
    id HYPpow(HYPn1?,HYPn2?pos_) = HYPnum(HYPn1)^HYPn2;
    id HYPpow(HYPn1?,HYPn2?neg_) = HYPden(HYPn1)^(-HYPn2);
  EndRepeat;
  Repeat;
    id,once HYPpow(HYPn1?,HYPn2?) = HYPpow(HYPn1)*HYPaux1(HYPn2);
    #call HypGetPolyCoefficients(HYPaux1,HYPep,HYPn1)
    id HYPaux1(?a) = HYPaux1(HYPaux(?a));
    Repeat;
      id HYPaux1(HYPaux(?a,HYPn1?),?b) = HYPaux1(HYPaux(?a),HYPn1*HYPep^nargs_(?a),?b);
    EndRepeat;
    id HYPpow(HYPn1?)*HYPaux1(HYPaux,?a) = HYPaux2(HYPn1,?a);
  EndRepeat;
  Transform,HYPaux2,AddArgs(3,last);
  Multiply Replace_(HYPaux2,HYPpow);
  Repeat;
    id HYPpow(HYPn1?,HYPn2?pos_,HYPn3?) = HYPnum(HYPn1)^HYPn2*HYPpowExpand(0,HYPn1,HYPn3/HYPep);
    id HYPpow(HYPn1?,HYPn2?neg_,HYPn3?) = HYPden(HYPn1)^(-HYPn2)*HYPpowExpand(0,HYPn1,HYPn3/HYPep);
    id HYPpow(HYPn1?,0,HYPn3?) = HYPpowExpand(0,HYPn1,HYPn3/HYPep);
  EndRepeat;
  #do i=1,1
    Argument HYPratep,2;
      $PowerBudget = `HYPMAXEP' + count_(HYPep,1);
      ModuleOption,Local,$PowerBudget;
    EndArgument;
    id,once HYPpowExpand(HYPn1?,HYPn2?,HYPn3?)*HYPratep(HYPn4?,HYPn5?) = HYPratep(invfac_(HYPn1)*HYPn3^HYPn1*HYPn4,HYPn5)*HYPlog(HYPn2)^HYPn1 +
      HYPratep(HYPep*HYPn4,HYPn5)*HYPpowExpand(HYPn1+1,HYPn2,HYPn3);
    Argument,HYPratep,1;
      If (count(HYPep,1) > $PowerBudget) Discard;
    EndArgument;
    id HYPratep(0,HYPn1?) = 0;
    if (count(HYPpowExpand,1) > 0) redefine i "0";
    .sort:HYPpowExpand;
  #enddo
  PolyRatFun HYPratep;
  .sort:HYPratep created;
#endprocedure
#procedure HypHyperExpand(Var,FuncID,PowCountFuncID,Count)
  .sort
  id `FuncID'(?a) = HYPcoeff(0,?a);
  #do i=1,1
    #if (`Count')
      $pow = count_(`PowCountFuncID',1);
    #else
      #$pow = `HYPMAXZ';
    #endif
    id HYPcoeff(HYPn1?,?a,HYPn2?!{0,}) = 
      -HYPden(HYPn2)*
       sum_(HYPn3,0,$pow-HYPn1-1,HYPden(HYPn2)^HYPn3/(HYPn1+HYPn3+1)*HYPcoeff(HYPn1+HYPn3+1,?a));
    also HYPcoeff(HYPn1?,?a,0) = 1/HYPn1*HYPcoeff(HYPn1,?a);
    id HYPcoeff(HYPn1?) = `Var'^HYPn1;
    if (count(HYPcoeff,1) > 0) redefine i "0";
    ModuleOption,local,$pow;
    .sort
  #enddo
#endprocedure
#procedure HypIntHlogDenZeroLetterRatFun(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (match(HYPdenx(0)) > 1) && 
         (match(HYPLx(HYPrat(HYPn1?,HYPn2?),?a)) > 0)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(0)^$pow*HYPLx(HYPrat(HYPn1?,HYPn2?),?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(0)^($pow-1)*HYPLx(HYPrat(HYPn1,HYPn2),?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*
          (
           sum_(HYPi,1,$pow-1,-HYPnum(HYPn2)^HYPi*HYPden(HYPn1)^HYPi*HYPdenx(0)^($pow-1-HYPi+1)) +
           HYPnum(HYPn2)^($pow-1)*HYPden(HYPn1)^($pow-1)*HYPdenx(HYPn1,HYPn2)
          )*
          HYPLx(?a)*HYPdx(`IntVar')
        ;
      endterm;
      moduleoption local $pow;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenLetterSamePoly(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) && 
         (count(HYPLx,1) == 1) &&
         (match(HYPdenx(HYPn1?)*HYPLx(HYPn1?,?a)) > 0)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?)^$pow*HYPLx(HYPn1?,?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1)^($pow-1)*HYPLx(HYPn1,?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*HYPdenx(HYPn1)^$pow*HYPLx(?a)*HYPdx(`IntVar')
        ;
      endterm;
      moduleoption local $pow;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenLetterSameRatFun(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) && 
         (match(HYPdenx(HYPn1?,HYPn2?)*HYPLx(HYPrat(HYPn1?,HYPn2?),?a)) > 0) &&
         (count(HYPLx,1) == 1)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?,HYPn2?)^$pow*HYPLx(HYPrat(HYPn1?,HYPn2?),?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1,HYPn2)^($pow-1)*HYPLx(HYPrat(HYPn1,HYPn2),?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*HYPdenx(HYPn1,HYPn2)^$pow*HYPLx(?a)*HYPdx(`IntVar')
        ;
      endterm;
      moduleoption local $pow;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenRatFunLetterPoly(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) && 
         (count(HYPLx,1) == 1) && 
         (match(HYPdenx(HYPn1?,HYPn2?)) != 0) &&
         (match(HYPLx(HYPn1?!{0,},?a)) != 0) &&
         (match(HYPLx(HYPrat(?a),?b)) == 0)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?,HYPn2?)^$pow*HYPLx(HYPn3?,?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1,HYPn2)^($pow-1)*HYPLx(HYPn3,?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*
          (
           sum_(HYPi,1,$pow-1,(-1)^(HYPi+1)*HYPden(HYPrat(HYPn1,HYPn2),HYPn3)^HYPi*HYPdenx(HYPn1,HYPn2)^($pow-1-HYPi+1)) +
           (-1)^($pow-1)*HYPden(HYPrat(HYPn1,HYPn2),HYPn3)^($pow-1)*HYPdenx(HYPn3)
          )*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      id HYPLx = 1;
      #call HypArgRatFunManip(HYPden,-)
      #call HypDenSimplify(HYPnum,HYPden,HYPrat,HYPz,HYPn1,HYPn2)
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenPolyLetterRatFun(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) && 
         (count(HYPLx,1) == 1) && 
         (match(HYPdenx(HYPn1?!{0,})) != 0) &&
         (match(HYPdenx(HYPn1?,HYPn2?)) == 0) &&
         (match(HYPLx(HYPrat(?a),?b)) == 1)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?)^$pow*HYPLx(HYPrat(HYPn2?,HYPn3?),?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1)^($pow-1)*HYPLx(HYPrat(HYPn2,HYPn3),?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*
          (
           sum_(HYPi,1,$pow-1,(-1)^(HYPi+1)*HYPden(HYPn1,HYPrat(HYPn2,HYPn3))^HYPi*HYPdenx(HYPn1)^($pow-1-HYPi+1)) +
           (-1)^($pow-1)*HYPden(HYPn1,HYPrat(HYPn2,HYPn3))^($pow-1)*HYPdenx(HYPn2,HYPn3)
          )*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      id HYPLx = 1;
      #call HypArgRatFunManip(HYPden,-)
      #call HypDenSimplify(HYPnum,HYPden,HYPrat,HYPz,HYPn1,HYPn2)
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenRatFunLetterRatFun(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) && 
         (
           (match(HYPdenx(HYPn1?!{HYPn3?},HYPn2?)*HYPLx(HYPrat(HYPn3?!{HYPn1?},HYPn4?),?a)) > 0) ||
           (match(HYPdenx(HYPn1?,HYPn2?!{HYPn4?})*HYPLx(HYPrat(HYPn3?,HYPn4?!{HYPn2?}),?a)) > 0)
         )
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?,HYPn2?)^$pow*
          HYPLx(HYPrat(HYPn3?,HYPn4?),?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1,HYPn2)^($pow-1)*HYPLx(HYPrat(HYPn3,HYPn4),?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*
          (
           sum_(HYPi,1,$pow-1,(-1)^(HYPi+1)*HYPden(HYPrat(HYPn1,HYPn2),HYPrat(HYPn3,HYPn4))^HYPi*HYPdenx(HYPn1,HYPn2)^($pow-1-HYPi+1)) +
           (-1)^($pow-1)*HYPden(HYPrat(HYPn1,HYPn2),HYPrat(HYPn3,HYPn4))^($pow-1)*HYPdenx(HYPn3,HYPn4)
          )*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      id HYPLx = 1;
      #call HypArgRatFunManip(HYPden,-)
      #call HypDenSimplify(HYPnum,HYPden,HYPrat,HYPz,HYPn1,HYPn2)
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenPolyLetterPoly(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) && 
         (count(HYPLx,1) > 0) && 
         (match(HYPdenx(HYPn1?,HYPn2?)) == 0) &&
         (match(HYPdenx(HYPn1?!{HYPn2?})*HYPLx(HYPn2?!{HYPn1?},?a)) != 0) &&
         (match(HYPLx(HYPrat(?a),?b)) == 0)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?!{HYPn2?})^$pow*HYPLx(HYPn2?!{HYPn1?},?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1)^($pow-1)*HYPLx(HYPn2,?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*
          (
           sum_(HYPi,1,$pow-1,(-1)^(HYPi+1)*HYPden(HYPn1-HYPn2)^HYPi*HYPdenx(HYPn1)^($pow-1-HYPi+1)) +
           (-1)^($pow-1)*HYPden(HYPn1-HYPn2)^($pow-1)*HYPdenx(HYPn2)
          )*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      id HYPLx = 1;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogDenRatFunLetterZero(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) > 1) &&
         (match(HYPdenx(HYPn1?,HYPn2?)) != 0) && 
         (match(HYPLx(0,?a)) != 0)
       );
      term;
        $pow = count_(HYPdenx,1);
        id HYPdenx(HYPn1?,HYPn2?)^$pow*HYPLx(0,?a)*HYPdx(`IntVar') =
          -1/($pow-1)*HYPdenx(HYPn1,HYPn2)^($pow-1)*HYPLx(0,?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) +
          1/($pow-1)*
          (
           sum_(HYPi,1,$pow-1,(-1)^(HYPi+1)*HYPnum(HYPn2)^HYPi*HYPden(HYPn1)^HYPi*HYPdenx(HYPn1,HYPn2)^($pow-1-HYPi+1)) +
           (-1)^($pow-1)*HYPnum(HYPn2)^($pow-1)*HYPden(HYPn1)^($pow-1)*HYPdenx(0)
          )*
          HYPLx(?a)*HYPdx(`IntVar')
        ;
      endterm;
      moduleoption local $pow;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogNoDenNoVarLetterZero(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) == 0) && 
         (count(`IntVar',1) == 0) && 
         (count(HYPLx,1) > 0) &&
         (match(HYPLx(0,?a)) > 0)
       );
      term;
        id HYPLx(0,?a)*HYPdx(`IntVar') = `IntVar'*HYPLx(0,?a)*
          (
            HYPlim(`IntVar',HYPinf) 
            #if (`DoZeroLimit')
              - 
              HYPlim(`IntVar',0)
            #endif
          ) -
          HYPLx(?a)*HYPdx(`IntVar')
        ;
      endterm;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogNoDenNoVarLetterPoly(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) == 0) && 
         (count(`IntVar',1) == 0) && 
         (match(HYPLx(HYPrat(?a),?b)) == 0) &&
         (match(HYPLx(0,?a)) == 0) &&
         (count(HYPLx,1) > 0)
       );
      id HYPLx(HYPn1?!{0,},?a)*HYPdx(`IntVar') = (`IntVar' - HYPnum(HYPn1))*HYPLx(HYPn1,?a)*
        (
          HYPlim(`IntVar',HYPinf) 
          #if (`DoZeroLimit')
            - 
            HYPlim(`IntVar',0)
          #endif
        ) -
        HYPLx(?a)*HYPdx(`IntVar')
      ;
      id HYPLx = 1;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogNoDenNoVarLetterRatFun(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1,`IntVar',1) == 0) && 
         (match(HYPLx(HYPrat(?a),?b)) != 0) &&
         (count(HYPLx,1) != 0)
       );
      id HYPLx(HYPrat(HYPn1?,HYPn2?),?a)*HYPdx(`IntVar') = 
        (`IntVar' - HYPnum(HYPn1)*HYPden(HYPn2))*
        HYPLx(HYPrat(HYPn1,HYPn2),?a)*
        (
          HYPlim(`IntVar',HYPinf) 
          #if (`DoZeroLimit')
            - 
            HYPlim(`IntVar',0)
          #endif
        ) -
        HYPLx(?a)*HYPdx(`IntVar')
      ;
      id HYPLx = 1;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogVarLetterZero(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) == 0) && 
         (count(`IntVar',1) > 0) && 
         (count(HYPLx,1) > 0) &&
         (match(HYPLx(0,?a)) > 0)
       );
      term;
        $pow = count_(`IntVar',1);
        id `IntVar'^$pow*HYPLx(0,?a)*HYPdx(`IntVar') = 
          1/($pow+1)*`IntVar'^($pow+1)*
            HYPLx(0,?a)*
            (
              HYPlim(`IntVar',HYPinf) 
              #if (`DoZeroLimit')
                - 
                HYPlim(`IntVar',0)
              #endif
            ) 
          -
          1/($pow+1)*`IntVar'^$pow*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogVarLetterPoly(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) == 0) && 
         (count(`IntVar',1) > 0) && 
         (match(HYPLx(HYPrat(?a),?b)) == 0) &&
         (count(HYPLx,1) > 0)
       );
      term;
        $pow = count_(`IntVar',1);
        id `IntVar'^$pow*HYPLx(HYPn1?!{0,},?a)*HYPdx(`IntVar') = 
          1/($pow+1)*(`IntVar'^($pow+1) - HYPnum(HYPn1)^($pow+1))*
            HYPLx(HYPn1,?a)*
            (
              HYPlim(`IntVar',HYPinf) 
              #if (`DoZeroLimit')
                - 
                HYPlim(`IntVar',0)
              #endif
            ) 
          -
          1/($pow+1)*sum_(HYPi,0,$pow,HYPnum(HYPn1)^(HYPi)*`IntVar'^($pow-HYPi))*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      id HYPLx = 1;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogVarLetterRatFun(IntVar)
  if (count(HYPdx,1) > 0);
    if (
         (count(HYPdenx,1) == 0) && 
         (count(`IntVar',1) > 0) && 
         (match(HYPLx(HYPrat(?a),?b)) > 0)
       );
      term;
        $pow = count_(`IntVar',1);
        id `IntVar'^$pow*HYPLx(HYPrat(HYPn1?,HYPn2?),?a)*HYPdx(`IntVar') = 
          1/($pow+1)*
            (
              `IntVar'^($pow+1) 
              - 
              HYPnum(HYPn1)^($pow+1)*HYPden(HYPn2)^($pow+1)
            )*
            HYPLx(HYPrat(HYPn1,HYPn2),?a)*
            (
              HYPlim(`IntVar',HYPinf) 
              #if (`DoZeroLimit')
                - 
                HYPlim(`IntVar',0)
              #endif
            ) 
          -
          1/($pow+1)*sum_(HYPi,0,$pow,HYPnum(HYPn1)^(HYPi)*HYPden(HYPn2)^(HYPi)*`IntVar'^($pow-HYPi))*HYPLx(?a)*HYPdx(`IntVar')
          ;
      endterm;
      moduleoption local $pow;
      id HYPLx = 1;
      #call HypSimplifyNumerics
    endif;
  endif;
#endprocedure
#procedure HypIntHlogs(IntVar)
  .sort:IntHlogs start;
  if ((count(`IntVar',1) > 0) && (count(HYPdenx,1) > 0));
    print "Tower cancellation did not happen...";
    print "%t";
    print "IntHlogs";
    exit;
  endif;
  #do i=1,1
    bracket HYPdx,HYPdenx,`IntVar',HYPLx;
    .sort:Bracketed in IntHlogs;
    keep brackets;
    #call HypIntHlogDenZeroLetterRatFun(`IntVar')
    #call HypIntHlogDenLetterSamePoly(`IntVar')
    #call HypIntHlogDenLetterSameRatFun(`IntVar')
    #call HypIntHlogDenRatFunLetterPoly(`IntVar')
    #call HypIntHlogDenPolyLetterRatFun(`IntVar')
    #call HypIntHlogDenRatFunLetterRatFun(`IntVar')
    #call HypIntHlogDenPolyLetterPoly(`IntVar')
    #call HypIntHlogDenRatFunLetterZero(`IntVar')
    #call HypIntHlogNoDenNoVarLetterZero(`IntVar')
    #call HypIntHlogNoDenNoVarLetterPoly(`IntVar')
    #call HypIntHlogNoDenNoVarLetterRatFun(`IntVar')
    #call HypIntHlogVarLetterZero(`IntVar')
    #call HypIntHlogVarLetterPoly(`IntVar')
    #call HypIntHlogVarLetterRatFun(`IntVar')
    #if 0
    #endif
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (match(HYPdenx(0)*HYPLx(HYPrat(HYPn1?,HYPn2?),?a)) > 0));
      redefine i "0";
    endif;
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (match(HYPdenx(HYPn1?)*HYPLx(HYPn1?,?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (match(HYPdenx(HYPn1?,HYPn2?)*HYPLx(HYPrat(HYPn1?,HYPn2?),?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (count(HYPLx,1) > 0) && 
        (match(HYPdenx(HYPn1?,HYPn2?)) > 0) && 
        (match(HYPLx(HYPrat(?a),?b)) == 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (count(HYPLx,1) > 0) && 
        (match(HYPdenx(HYPn1?,HYPn2?)) == 0) && 
        (match(HYPLx(HYPrat(?a),?b)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (
          (match(HYPdenx(HYPn1?!{HYPn3?},HYPn2?)*HYPLx(HYPrat(HYPn3?!{HYPn1?},HYPn4?),?a)) > 0) ||
          (match(HYPdenx(HYPn1?,HYPn2?!{HYPn4?})*HYPLx(HYPrat(HYPn3?,HYPn4?!{HYPn2?}),?a)) > 0)
        )
       ) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (match(HYPdenx(HYPn1?,HYPn2?)) == 0) &&
        (match(HYPLx(HYPrat(?a),?b)) == 0) &&
        (match(HYPdenx(HYPn1?!{HYPn2?})*HYPLx(HYPn2?!{HYPn1?},?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) > 1) && 
        (match(HYPdenx(HYPn1?,HYPn2?)) > 0) &&
        (match(HYPLx(0,?b)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1,`IntVar',1) == 0) && 
        (match(HYPLx(0,?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1,`IntVar',1) == 0) && 
        (match(HYPLx(HYPrat(?a),?b)) == 0) &&
        (match(HYPLx(HYPn1?!{0,},?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1,`IntVar',1) == 0) && 
        (match(HYPLx(HYPrat(?a),?b)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) == 0) && 
        (count(`IntVar',1) > 0) && 
        (match(HYPLx(0,?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) == 0) && 
        (count(`IntVar',1) > 0) && 
        (match(HYPLx(HYPrat(?a),?b)) == 0) &&
        (match(HYPLx(HYPn1?!{0,},?a)) > 0)) redefine i "0";
    if ((count(HYPdx,1) > 0) && 
        (count(HYPdenx,1) == 0) && 
        (count(`IntVar',1) > 0) && 
        (match(HYPLx(HYPrat(?a),?b)) > 0)) redefine i "0";
    ModuleOption,local,$pow;
    .sort:End of main loop;
  #enddo
  if (count(HYPdx,1) > 0);
    if ((count(HYPdenx,1) == 1) && (count(HYPLx,1) == 1));
      id HYPdenx(HYPn1?)*HYPLx(?a)*HYPdx(`IntVar') = 
        (
          HYPlim(`IntVar',HYPinf) 
          #if (`DoZeroLimit')
          - HYPlim(`IntVar',0)
          #endif
        )*HYPLx(HYPn1,?a);
      id HYPdenx(HYPn1?,HYPn2?)*HYPLx(?a)*HYPdx(`IntVar') = 
        (
          HYPlim(`IntVar',HYPinf) 
          #if (`DoZeroLimit')
          - HYPlim(`IntVar',0)
          #endif
        )*HYPLx(HYPrat(HYPn1,HYPn2),?a);
    endif;
  endif;
  if ((count(HYPdx,1) > 0) && (count(HYPLx,1) > 0));
    print "Still have hyperlogs to integrate: ";
    print "%t";
    print "IntHlogs...";
    exit;
  endif;
  #if 0
  #endif
#endprocedure
#procedure HypIntRatFun(Var)
  if ((match(HYPdenx(HYPn1?!{HYPn2?})*
             HYPdenx(HYPn2?!{HYPn1?})) > 0) ||
      (match(HYPdenx(HYPn1?,HYPn2?)*
             HYPdenx(HYPn3?)) > 0) ||
      (match(HYPdenx(HYPn1?!{HYPn3?},HYPn2?!{HYPn4?})*
             HYPdenx(HYPn3?!{HYPn1?},HYPn4?!{HYPn2?})) > 0));
    print "Term is not suitable for integration: ";
    print "%t";
    print "IntRatFun...";
    exit;
  endif;
  if ((count(HYPdenx,1) > 0) && (count(HYPLx,1) == 0) && (count(HYPdx,1) > 0));
    $pow = count_(HYPdenx,1);
    if ($pow > 1);
      id HYPdenx(HYPn1?)^$pow*HYPdx(`Var') = 
        1/(-$pow+1)*HYPdenx(HYPn1)^($pow-1)*
        (
          HYPlim(`Var',HYPinf) 
          - 
          HYPlim(`Var',0)
        )
      ;
      id HYPdenx(HYPn1?,HYPn2?)^$pow*HYPdx(`Var') = 
        1/(-$pow+1)*HYPdenx(HYPn1,HYPn2)^($pow-1)*
        (
          HYPlim(`Var',HYPinf) 
          - 
          HYPlim(`Var',0)
          )
      ;
    else;
      id HYPdenx(0)*HYPdx(`Var') = 
        HYPLx(0)*
        (
          HYPlim(`Var',HYPinf) 
          #if (`DoZeroLimit')
            - 
            HYPlim(`Var',0)
          #endif
        )
      ;
      id HYPdenx(HYPn1?!{0,})*HYPdx(`Var') = 
        (0*HYPlog(-HYPn1) + HYPLx(HYPn1))*
        (
          HYPlim(`Var',HYPinf) 
          #if (`DoZeroLimit')
            - 
            HYPlim(`Var',0)
          #endif
        )
      ;
      id HYPdenx(HYPn1?,HYPn2?)*HYPdx(`Var') = 
        (0*HYPlog(-HYPn1) - 0*HYPlog(HYPn2) + HYPLx(HYPrat(HYPn1,HYPn2)))*
        (
          HYPlim(`Var',HYPinf) 
          #if (`DoZeroLimit')
            - 
            HYPlim(`Var',0)
          #endif
        )
      ;
    endif;
  endif;
  if (
      (count(HYPdenx,1) == 0) && 
      (count(HYPLx,1) == 0) && 
      (count(HYPdx,1) > 0)
     );
    $pow = count_(`Var',1);
    id `Var'^$pow*HYPdx(`Var') = 
      1/($pow+1)*`Var'^($pow+1)*
      (
        HYPlim(`Var',HYPinf) 
        - 
        HYPlim(`Var',0)
      )
    ;
  endif;
  ModuleOption,local,$pow;
  .sort:IntRatFun;
#endprocedure
#procedure HypApplyChenWu(ExprID,IntVar)
  InExpression `ExprID';
    #ifdef ``IntVar''
      multiply replace_(HYPz``IntVar'', 1);
    #else
      multiply replace_(HYPz`IntVar', 1);
    #endif
  EndInExpression;
#endprocedure
#procedure HypSetInternalIntegrationSequence(?Sequence)
  #If ("`HYPIntegrationSequence'" == "")
    #Define First "1"
    #Do var={`?Sequence',}
      #If ("`var'" != "")
        #If (`First' != 1)
          #Redefine HYPIntegrationSequence "`HYPIntegrationSequence',"
        #Else
          #Redefine First "0"
        #Endif
        #Redefine HYPIntegrationSequence "`HYPIntegrationSequence'HYPz`var'"
      #Endif
    #Enddo
  #Endif
#endprocedure
#procedure HypIntegrationStepV1(ExprID,IntVarOrdNumber)
  .sort:IntegrationStep start;
  #define IntVar "HYPz`IntVarOrdNumber'"
  #define ACTIVETMP "`activeexprnames_'"
  hide;
  .sort:hid everything;
  unhide `ExprID';
  .sort:Unhid `ExprID';
  #message Integrating `IntVar' in `ExprID';
  #call HypSimplify
  .sort:HYPratep turned off;
  PolyRatFun;
  #call HypLimitsToHlogs(`ExprID',HYPLinfRegInfZero,`IntVar')
  PolyRatFun HYPratep;
  .sort:HYPratep back on;
  #call HypRevealVar(`IntVar')
  #call HypToHyperlog
  #call HypSimplify
  #if (`HYPcheckForZeroCoefficients')
    .sort
    PolyRatFun;
    #if ("`HYPIntegrationSequence'" == "")
      #call HypSetInternalIntegrationSequence(`IntegrationSequence')
    #endif
    #call HypCheckForZeroCoeffs(`ExprID',HYPLx,HYPtempCount,`IntVar',`HYPIntegrationSequence')
    .sort
    PolyRatFun HYPratep;
  #endif
  #call HypDecomposeWRTvar(`IntVar')
  #call HypSimplify
  multiply HYPdx(`IntVar');
  #call HypIntHlogs(`IntVar')
  #call HypIntRatFun(`IntVar')
  .sort
  #call HypDenSimplify(HYPnum,HYPden,HYPrat,HYPz,HYPn1,HYPn2)
  #call HypTakeLimitInf(`IntVar')
  #call HypTakeLimitZero(`IntVar')
  #call HypSimplify
  #call HypFromLimitHlogsToNumbers(`ExprID',HYPLinfRegInfZero,HYPLinf)
  shuffle HYPLlimZero;
  .sort:Hid `ExprID';
  hide `ExprID';
  .sort:Unhiding original exprs;
  unhide `ACTIVETMP';
  .sort:IntegrationStep fin;
  #if 0
  #endif
#endprocedure
#procedure HypSimpleIntegrate(DenFuncID,HlogFuncID,RatFuncID,UseTag,TagFuncID,RemoveTag,Sym1,Sym2)
  if (
    #if (`UseTag')
      count(`TagFuncID',1) > 0
    #else
      1
    #endif
  );
    if ((count(`DenFuncID',1) > 1) || (count(`DenFuncID',1) == 0) || (count(`HlogFuncID',1) > 1));
      print "Ambiguos term for SimpleIntegrate...";
      print "Term: %t";
      exit "SimpleIntegrate";
    endif;
    if (count(`HlogFuncID',1) == 0);
      multiply `HlogFuncID';
    endif;
    id `DenFuncID'(`Sym1'?)*`HlogFuncID'(?a) = `HlogFuncID'(`Sym1',?a);
    also `DenFuncID'(`Sym1'?,`Sym2'?)*`HlogFuncID'(?a) = `HlogFuncID'(`RatFuncID'(`Sym1',`Sym2'),?a);
    #if (`RemoveTag')
      id `TagFuncID'(?a) = 1;
    #endif
  endif;
#endprocedure
#procedure HypIntegrateAtLevelSubstituteAbove(ExprID,MinLevel,MaxLevel,IntegratedFuncID,LHSfuncID,DenFuncID,RatFuncID,AuxFuncID,IntMeasureID,Sym1,Sym2,?RHSfuncs)
  .sort:IntegrateAtLevelSubstituteAbove starts;
  #do level=`MinLevel',`MaxLevel'
    #if (`HYPdebug')
      #message INTEGRATION AT LEVEL `level';
    #endif
    skip;
    nskip `ExprID'`level';
    #call HypSimpleIntegrate(`DenFuncID',`IntegratedFuncID',`RatFuncID',1,`IntMeasureID',1,`Sym1',`Sym2')
    #if (`level' < `MaxLevel')
      #if (`HYPdebug')
        #message SUBSTITUTION TO <`ExprID'{`level'+1}>,...,<`ExprID'`MaxLevel'>;
      #endif
      #do RHSfunc={`?RHSfuncs'}
        #call HypSubstituteToExprsWithExtrasAndTable(`ExprID'`level',`LHSfuncID',`RHSfunc',`AuxFuncID',HYPsolutionTbl,<`ExprID'{`level'+1}>,...,<`ExprID'`MaxLevel'>)
      #enddo
      skip;
    #endif
    .sort:"Level `level' finished";
  #enddo
#endprocedure
#procedure HypDissectExprWithArgNumber(ExprID, DissectExprID, FuncID, TagFuncID, Sym1, MinArgNum, MaxArgNum, RemoveOriginal, HideTags)
  .sort:DissectExprWithArgNumber start;
  skip;
  nskip `ExprID';
  id `FuncID'(?a) = `TagFuncID'(nargs_(?a))*`FuncID'(?a);
  #$mintag = 99;
  #$maxtag = 0;
  if (match(`TagFuncID'(`Sym1'?$tmp)) > 0);
    if ($mintag > $tmp) $mintag = $tmp;
    if ($maxtag < $tmp) $maxtag = $tmp;
  endif;
  ModuleOption,local,$tmp;
  ModuleOption,minimum,$mintag;
  ModuleOption,maximum,$maxtag;
  .sort:min/max tags collected;
  #redefine `MinArgNum' "`$mintag'"
  #redefine `MaxArgNum' "`$maxtag'"
  #if (`HYPdebug')
    #write "Minimum and maximum tags: %$ %$", $mintag, $maxtag;
  #endif
  CreateSpectator TagSpectator, "tag.spec";
  #do itag=``MinArgNum'', ``MaxArgNum''
    skip;
    nskip `ExprID';
    #if (`HYPdebug')
      #message Storing tags at level `itag';
    #endif
    if (match(`TagFuncID'(`itag')) > 0) ToSpectator TagSpectator;
    .sort:Tags at `itag' collected;
    skip;
    CopySpectator `DissectExprID'`itag' = TagSpectator;
    if (expression(`DissectExprID'`itag')) id `TagFuncID'(`Sym1'?) = 1;
    .sort:`DissectExprID'`itag' created;
    skip;
    EmptySpectator TagSpectator;
    .sort:Spectator emptied;
  #enddo
  skip;
  RemoveSpectator TagSpectator;
  .sort:Spectator removed;
  skip;
  #if (`RemoveOriginal')
    skip;
    drop `ExprID';
    .sort:`ExprID' got dropped;
  #endif
  #if (`HideTags')
    skip;
    hide <`DissectExprID'``MinArgNum''>,...,<`DissectExprID'``MaxArgNum''>;
    .sort:Dissected exprs are hidden;
  #endif
#endprocedure
#procedure HypMergeLevelExpressions(ExprID,ToMergedExprID,MinLevel,MaxLevel,DeleteFlag)
  .sort:MergeLevelExpressions start;
  #do ilevel=`MinLevel',`MaxLevel'
    skip;
    #if (exists(`ToMergedExprID'`ilevel'))
      #if (exists(`ExprID'`ilevel'))
        local `ExprID'`ilevel' = `ExprID'`ilevel' + `ToMergedExprID'`ilevel';
        .sort:`ExprID'`ilevel' and `ToMergedExprID'`ilevel' merged;
        #if (`DeleteFlag')
          skip;
          drop `ToMergedExprID'`ilevel';
          .sort:`ToMergedExprID'`ilevel' dropped;
        #endif
      #endif
    #endif
  #enddo
#endprocedure
#procedure HypShuffleNonNumericHlogs(FuncID,AuxFuncID,Sym,MoveBack)
  id `FuncID'(?a,`Sym'?!number_,?b) = `AuxFuncID'(?a,`Sym',?b);
  shuffle `AuxFuncID';
  #if (`MoveBack' == 1)
    multiply replace_(`AuxFuncID', `FuncID');
  #endif
  .sort:Non-num `FuncID's shuffled;
#endprocedure
#procedure HypShuffleToNonZeroEnd(FuncID)
  #do i=1,1
    id,once `FuncID'(?a,0) = HYPaux(?a,0);
    id HYPaux(?a,0) = HYPaux(?a)*HYPaux1(0);
    #do j=1,1
      repeat id HYPaux(?a,0)*HYPaux1(?b) = HYPaux(?a)*HYPaux1(?b,0);
      id HYPaux*HYPaux1(?a) = HYPaux2(?a);
      id HYPaux(?a)*HYPaux1(?b) = HYPaux2(?a)*HYPaux2(?b) - HYPaux1(?a)*HYPaux1(?b) + HYPaux1(?a,?b);
      shuffle HYPaux1;
      if (match(HYPaux1(?a,0)) > 0);
        redefine j "0";
        id HYPaux1(?a,0) = HYPaux(?a)*HYPaux1(0);
      endif;
      .sort
    #enddo
    multiply replace_(HYPaux1,HYPaux2);
    if (match(`FuncID'(?a,0)) > 0) redefine i "0";
    .sort
  #enddo
  multiply replace_(HYPaux2,`FuncID');
#endprocedure
#procedure HypTakeLimitInf(Var)
  #call HypVarSimplify(`Var')
  #call HypLimInfToLimZero(`Var')
#endprocedure
#procedure HypTakeLimitZero(Var)
  #call HypVarSimplify(`Var')
  if (match(HYPlim(`Var',0)) > 0);
    multiply replace_(HYPLx,HYPL0);
  endif;
  #call HypShuffleToNonZeroEnd(HYPL0)
  id HYPL0(?a,0) = HYPLx(?a,0);
  #call HypHyperExpand(`Var',HYPL0,HYPdenx,1)
  #call HypVarSimplify(`Var')
  #call HypSimplifyNumerics
  .sort
  if (count(HYPLx,1,HYPdenx,1,HYPlogx,1,`Var',1) == 0);
    id HYPlim(`Var',0) = 1;
  elseif (match(HYPlim(`Var',0)) > 0);
    #if (`HYPDEBUG')
      print "Found zero limit...";
      print "%t";
      print "Mapping it away...";
    #endif
    multiply replace_(HYPLx,HYPLlimZero);
    id HYPdenx(0) = HYPinf;
    id HYPlim(`Var',0) = 1;
  endif;
  .sort
  #if 0
  if (match(HYPLlimZero(?a,HYPn1?!number_,?b)) > 0);
    print "Found survining zero-limit hyperlog with non-numeric argument...";
    print "%t";
    exit "TakeLimitZero";
  endif;
  #endif
#endprocedure
#procedure HypChangeFunctionHeadForVarDependentInstances(ExprID, FuncID, VarDepFuncID, RatFuncID, PolyFuncID, Sym1, Sym2)
  if (expression(`ExprID'));
    id `FuncID'(?a,`RatFuncID'(`PolyFuncID'(`Sym1'?,?b,`Sym2'?!{0,},?c),`PolyFuncID'(?d)),?e) = 
      `VarDepFuncID'(?a,`RatFuncID'(`PolyFuncID'(`Sym1',?b,`Sym2',?c),`PolyFuncID'(?d)),?e);
    id `FuncID'(?a,`RatFuncID'(`PolyFuncID'(?b),`PolyFuncID'(`Sym1'?,?c,`Sym2'?!{0,},?d)),?e) = 
      `VarDepFuncID'(?a,`RatFuncID'(`PolyFuncID'(?b), `PolyFuncID'(`Sym1',?c,`Sym2',?d)),?e);
  endif;
#endprocedure
#procedure HypChangeFuncHeadForGivenWordLengthAndVarDependence(ExprID,FuncID,NewFuncID,AuxFuncID,RatFuncID,PolyFuncID,Sym1,Sym2,WordLength)
  if (expression(`ExprID'));
    id `FuncID'(?a) = `FuncID'(nargs_(?a), ?a);
    id `FuncID'(`WordLength', ?a) = `AuxFuncID'(?a);
    id `FuncID'(`Sym1'?number_, ?a) = `FuncID'(?a);
  endif;
  #call HypChangeFunctionHeadForVarDependentInstances(`ExprID', `AuxFuncID', `NewFuncID', `RatFuncID', `PolyFuncID', `Sym1', `Sym2')
  if (expression(`ExprID'));
    multiply replace_(`AuxFuncID', `FuncID');
  endif;
#endprocedure
#procedure HypDropSubLeadTermsRatFuncArg(FuncID,RatFuncID,PolyFuncID,n1,...,n2)
  argument `FuncID';
    argument `RatFuncID';
      repeat;
        id `PolyFuncID'(?a,`n1'?!{0,},`n2'?,?b) = `PolyFuncID'(?a,`n1');
      endrepeat;
    endargument;
  endargument;
#endprocedure
#procedure HypDetermineVarOrderRatFuncArgs(FuncID,RatFuncID,PolyFuncID)
  argument `FuncID';
    id `RatFuncID'(`PolyFuncID'(?a),`PolyFuncID'(?b)) = 
      `RatFuncID'(nargs_(?a) - nargs_(?b),`PolyFuncID'(?a),`PolyFuncID'(?b));
  endargument;
#endprocedure
#procedure HypTakeZeroLimitInLimitHlog(ExprID,FuncID,RatFuncID,PolyFuncID,ConstRatFuncID,<Aux1FuncID>,...,<Aux3FuncID>,FinFuncID,n1,...,n3)
  .sort:TakeZeroLimitInLimitHlog start;
  skip;
  nskip `ExprID';
  #call HypDropSubLeadTermsRatFuncArg(`FuncID',`RatFuncID',`PolyFuncID',`n1',...,`n2')
  #call HypDetermineVarOrderRatFuncArgs(`FuncID',`RatFuncID',`PolyFuncID')
  argument `FuncID';
    id `RatFuncID'(0,`PolyFuncID'(?a,`n1'?),`PolyFuncID'(?b,`n2'?)) = `ConstRatFuncID'(`n1',`n2');
  endargument;
  multiply replace_(`FuncID',`Aux1FuncID');
  id `Aux1FuncID'(?a,`RatFuncID'(?b),?c) = `FuncID'(?a,`RatFuncID'(?b),?c);
  multiply replace_(`Aux1FuncID',`FinFuncID');
  multiply replace_(`FuncID',`Aux1FuncID');
  .sort:Functionheads adjusted for shuffling;
  #define ShuffleLoopCount "0"
  #do iZero=1,1
    skip;
    nskip `ExprID';
    #redefine ShuffleLoopCount "{`ShuffleLoopCount'+1}"
    #if (`HYPdebug')
      #message "ShuffleLoopCount got incremented to `ShuffleLoopCount'"
    #endif
    id,once `Aux1FuncID'(?a) = `Aux2FuncID'(?a);
    if (count(`Aux2FuncID',1) > 0);
      $varord = 99;
      argument `Aux2FuncID';
        if (match(`RatFuncID'(`n1'?$tmp,?a)) > 0);
          if ($tmp < $varord) $varord = $tmp;
        endif;
      endargument;
      argument `Aux2FuncID';
        if (($varord < 99) && ($varord > 0));
          if (match(`ConstRatFuncID'(`n1'?!{0,},`n2'?)) > 0);
            $varord = 0;
          endif;
        endif;
      endargument;
      if ($varord < 99);
        id `Aux2FuncID'(?a) = `Aux2FuncID'($varord,?a);
      else;
        id `Aux2FuncID'(?a) = `FinFuncID'(?a);
      endif;
      id `Aux2FuncID'(`n1'?,?a,`RatFuncID'(`n1'?,?b)) = `Aux3FuncID'(`n1',?a,`RatFuncID'(`n1',?b));
      also `Aux2FuncID'(0,?a,`ConstRatFuncID'(`n1'?!{0,},`n2'?)) = `Aux3FuncID'(0,?a,`ConstRatFuncID'(`n1',`n2'));
      if (match(`Aux2FuncID'(0,?a,`ConstRatFuncID'(`n1'?!{0,},`n2'?),?b)) == 0);
        id `Aux2FuncID'(0,?a) = `Aux3FuncID'(0,?a);
      endif;
      id `Aux2FuncID'(0,?a,`ConstRatFuncID'(?b)) = `Aux3FuncID'(0,?a,`ConstRatFuncID'(?b));
      id `Aux2FuncID'(`n1'?!{`n2'?},?a,`RatFuncID'(`n2'?!{`n1'?},?b)) = 
        thetap_(`n2'-`n1')*`Aux2FuncID'(`n1',?a)*`Aux1FuncID'(`RatFuncID'(`n2',?b))
        +
        theta_(`n1'-`n2')*`Aux3FuncID'(`n1',?a,`RatFuncID'(`n2',?b))
      ;
      also `Aux2FuncID'(`n1'?neg_,?a,`RatFuncID'(`n1'?,?b),?c,`RatFuncID'(`n2'?!{`n1'?},?d),?e) = 
        `Aux2FuncID'(`n1',?a,`RatFuncID'(`n1',?b),?c)*`Aux1FuncID'(`RatFuncID'(`n2',?d),?e)
      ;
      also `Aux2FuncID'(`n1'?neg_,?a,`RatFuncID'(`n1'?,?b),?c,`ConstRatFuncID'(?d),?e) = 
        `Aux2FuncID'(`n1',?a,`RatFuncID'(`n1',?b),?c)*`Aux1FuncID'(`ConstRatFuncID'(?d),?e)
      ;
      repeat;
        if (
          (count(`Aux1FuncID',1) > 0)
        );
          id `Aux2FuncID'(`n1'?!{`n2'?},?a,`RatFuncID'(`n2'?!{`n1'?},?b))*`Aux1FuncID'(?c) = 
            thetap_(`n2'-`n1')*`Aux2FuncID'(`n1',?a)*`Aux1FuncID'(`RatFuncID'(`n2',?b),?c)
            +
            theta_(`n1'-`n2')*`Aux2FuncID'(`n1',?a)*`Aux2FuncID'(`RatFuncID'(`n2',?b),?c)
          ;
          also `Aux2FuncID'(`n1'?,?a,`ConstRatFuncID'(0, 1))*`Aux1FuncID'(?c) = 
            `Aux2FuncID'(`n1',?a)*`Aux1FuncID'(`ConstRatFuncID'(0, 1),?c)
          ;
          also `Aux2FuncID'(`n1'?neg_,?a,`RatFuncID'(`n1'?,?b),?c,`RatFuncID'(`n2'?!{`n1'?},?d))*`Aux1FuncID'(?f) = 
            `Aux2FuncID'(`n1',?a,`RatFuncID'(`n1',?b),?c)*`Aux1FuncID'(`RatFuncID'(`n2',?d),?f)
          ;
          also `Aux2FuncID'(`n1'?neg_,?a,`RatFuncID'(`n1'?,?b),?c,`ConstRatFuncID'(?d))*`Aux1FuncID'(?f) = 
            `Aux2FuncID'(`n1',?a,`RatFuncID'(`n1',?b),?c)*`Aux1FuncID'(`ConstRatFuncID'(?d),?f)
          ;
          also `Aux2FuncID'(`n1'?pos0_,?a,`ConstRatFuncID'(`n2'?!{0,},`n3'?))*`Aux1FuncID'(?c) = 
            `Aux1FuncID'(?a,`ConstRatFuncID'(`n2',`n3'))*`Aux1FuncID'(?c) -
            `Aux2FuncID'(?a,`ConstRatFuncID'(`n2',`n3'))*`Aux2FuncID'(?c) +
            `Aux1FuncID'(?a,`ConstRatFuncID'(`n2',`n3'),?c)
          ;
          also `Aux2FuncID'(`n1'?neg_,?a,`RatFuncID'(`n1'?neg_,?b))*`Aux1FuncID'(?c) = 
            `Aux1FuncID'(?a,`RatFuncID'(`n1',?b))*`Aux1FuncID'(?c) -
            `Aux2FuncID'(?a,`RatFuncID'(`n1',?b))*`Aux2FuncID'(?c) +
            `Aux1FuncID'(?a,`RatFuncID'(`n1',?b),?c)
          ;
          #if 0
          #endif
        endif;
      endrepeat;
      #if 0
      #endif
    endif;
    shuffle `Aux2FuncID';
    multiply replace_(`Aux2FuncID',`Aux1FuncID');
    ModuleOption,local,$varord,$tmp;
    .sort:After shuffle;
    skip;
    nskip `ExprID';
    if (count(`Aux1FuncID',1) > 0) redefine iZero "0";
    #if (`HYPdebug')
      #if (`ShuffleLoopCount' > 10)
        if (count(`Aux1FuncID',1) > 0);
          print "ShuffleLoopCount is over 10, due to term: ";
          print "---> %t";
        endif;
      #endif
    #endif
    #if 0
    #endif
    .sort:ZeroShuffle loop;
  #enddo
  skip;
  nskip `ExprID';
  #if (`HYPdebug')
    #message "Shuffle loop finished after `ShuffleLoopCount' iterations"
  #endif
  if ((match(`Aux3FuncID'(`n1'?!{`n2'?},?a,`RatFuncID'(`n2'?!{`n1'?},?b)))) ||
      (match(`Aux3FuncID'(`n1'?{<0,},?a,`ConstRatFuncID'(?b)))) ||
      (match(`Aux3FuncID'(`n1'?{>0,},?a,`ConstRatFuncID'(?b),?b))));
    print "Argument of Linf was not regularized properly...";
    print "TERM: %t";
    exit "TakeZeroLimitInLimitHlog";
  endif;
  repeat;
    id,once `Aux3FuncID'(`n1'?$deg,?a) = `Aux1FuncID'($deg,?a);
    if (count(`Aux1FuncID',1) > 0);
      if ($deg < 0);
        argument `Aux1FuncID';
          id `ConstRatFuncID'(?a) = `ConstRatFuncID'(0, 1);
        endargument;
      endif;
      argument `Aux1FuncID';
        id `RatFuncID'(`n1'?,?a) = `RatFuncID'(`n1'-$deg,?a);
        id `RatFuncID'(`n1'?{>0,},?a) = `ConstRatFuncID'(0, 1);
        id `RatFuncID'(0,`PolyFuncID'(?a,`n1'?),`PolyFuncID'(?b,`n2'?)) = `ConstRatFuncID'(`n1',`n2');;
      endargument;
      multiply replace_(`Aux1FuncID',`Aux2FuncID');
    endif;
  endrepeat;
  ModuleOption,local,$deg;
  .sort:Rescale over;
  skip;
  nskip `ExprID';
  id `Aux2FuncID'(`n1'?,?a) = `FinFuncID'(?a);
  #if 0
  #endif
  .sort:TakeZeroLimitInLimitHlog fin;
#endprocedure
#procedure HypPartingForLimitDiffs(ExprID,FuncID,DiffFuncID,UseFilter,FilterFuncID,AuxFuncID,Sym1,...,Sym4)
  .sort:PartingForLimitDiffs starts;
  skip;
  nskip `ExprID';
  if (
    #if (`UseFilter')
      count(`FilterFuncID',1) > 0
    #else
      1
    #endif
  );
    id `FuncID'(?args,`Sym1'?) = 
      -`DiffFuncID'(`Sym1')*`FuncID'(?args) 
      + sum_(`Sym2',1,nargs_(?args),
             (`DiffFuncID'(`Sym2'+1,`Sym2',`Sym2'+1) - `DiffFuncID'(`Sym2',`Sym2',`Sym2'+1))*`FuncID'(?args,`Sym1'))
    ;
  endif;
  .sort:Initial diff sum formed;
  skip;
  nskip `ExprID';
  if (
    #if (`UseFilter')
      count(`FilterFuncID',1) > 0
    #else
      1
    #endif
  );
    if (match(`DiffFuncID'(`Sym1'?$m,`Sym2'?$n,`Sym3'?$np1)) > 0);
      $mp1 = $m + 1;
      id `FuncID'(?a) = `FuncID'(`Sym4',?a);
      transform,`FuncID',permute(1,$mp1);
      id `DiffFuncID'(`Sym1'?,`Sym2'?,?a)*`FuncID'(`Sym3'?,?b) = 
        `DiffFuncID'(`Sym1',`Sym2',?a,`Sym3')*`FuncID'(?b);
      transform,`FuncID',dropargs($m,$m);
      transform,`FuncID',permute(1,$n);
      id `DiffFuncID'(`Sym1'?,`Sym1'?,`Sym2'?,`Sym3'?)*`FuncID'(`Sym4'?,?a) = 
         `DiffFuncID'(`Sym3',`Sym4')*`FuncID'(`Sym4',?a);
      id `DiffFuncID'(`Sym1'?,`Sym2'?,`Sym1'?,`Sym3'?)*`FuncID'(`Sym4'?,?a) = 
         `DiffFuncID'(`Sym4',`Sym3')*`FuncID'(`Sym4',?a);
      transform,`FuncID',permute(1,$n);
    endif;
  endif;
  ModuleOption,local,$m,$mp1,$n,$np1;
  .sort:PartingForLimitDiffs fin;
#endprocedure
#procedure HypEliminateVarDepPolysForIndepFuncs(FuncID,RatFuncID,PolyFuncID,AuxFuncID,Sym1,Sym2)
  repeat;
    id `FuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1'?,?c,`Sym2'?!{0,},?d),?e),?f) = `AuxFuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1',?c,`Sym2',?d),?e),?f)
    ;
  endrepeat;
  argument `FuncID';
    argument `RatFuncID'; transform,`PolyFuncID',addargs(1,last);
    endargument;
  endargument;
  multiply replace_(`AuxFuncID', `FuncID');
#endprocedure
#procedure HypEliminateRatFuncArgsForNumericArgs(FuncID,RatFuncID,PolyFuncID,AuxFuncID,Sym1,Sym2)
  id `FuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1'?,?c,`Sym2'?),?d),?e) =
    `AuxFuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1',?c,`Sym2'),?d),?e)
  ;
  id `FuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1'?!number_),?c),?d) =
    `AuxFuncID'(?a,`RatFuncID'(?b,`PolyFuncID'(`Sym1'),?c),?d)
  ;
  argument `FuncID';
    argument `RatFuncID';
      id `PolyFuncID'(`Sym1'?number_) = `Sym1';
    endargument;
    id `RatFuncID'(`Sym1'?number_,`Sym2'?number_) = `Sym1'/`Sym2';
  endargument;
  multiply replace_(`AuxFuncID',`FuncID');
#endprocedure
#procedure HypDiffLogDiffs(ExprID,DiffFuncID,RatFuncID,PolyFuncID,DenFuncID,Sym1,...,Sym4)
  .sort:DiffLogDiffs start;
  skip;
  nskip `ExprID';
  id `DiffFuncID'(`Sym1'?,`Sym2'?) = `DenFuncID'(-`Sym1',`Sym2');
  id `DiffFuncID'(`Sym1'?,`Sym2'?,`Sym3'?,`Sym4'?) = `DenFuncID'(-`Sym1',`Sym2') 
                                                   - `DenFuncID'(-`Sym3',`Sym4')
  ;
#endprocedure
#procedure HypDifferentiateHlogsAtLevel(ExprID,FuncID,RatFuncID,PolyFuncID,Var,Level,UseFilter,FilterFuncID,IsolateForZeroLimit,ZeroLimitFuncID,ConstFuncID)
  #call HypChangeFuncHeadForGivenWordLengthAndVarDependence(`ExprID',`FuncID',HYPtoDiff,HYPaux,`RatFuncID',`PolyFuncID',HYPn1,HYPn2,`Level')
  #if (`IsolateForZeroLimit')
    id HYPtoDiff(?a) = HYPtoDiff(?a) + `ZeroLimitFuncID'(?a);
    #if (`FibrationBasisZeroLimitBeforeDiff')
      #call HypTakeZeroLimitInLimitHlog(`ExprID',`ZeroLimitFuncID',`RatFuncID',`PolyFuncID',HYPcrat,HYPaux1,...,HYPaux3,HYPLconst,HYPn1,...,HYPn3)
      #call HypSimplifyRatFuncsTrivial(HYPLconst,HYPcrat,HYPcrat,HYPpoly,HYPn1,HYPn2)
      #call HypSimplifyRatFuncArguments(`ExprID',HYPLconst, HYPcrat, HYPaux, HYPdiv1, HYPdiv2, HYPgcd, HYPsimpTbl, HYPn1, HYPn2, HYPn3)
      #call HypFromLimitHlogsToNumbers(`ExprID',HYPLconst,HYPLinf)
      skip;
      nskip `ExprID';
      multiply replace_(HYPLconst,`ConstFuncID');
      argument `ConstFuncID';
        multiply replace_(HYPcrat,`RatFuncID');
      endargument;
    #endif
  #endif
  #call HypPartingForLimitDiffs(`ExprID',HYPtoDiff,HYPdiff,`UseFilter',`FilterFuncID',HYPaux,HYPn1,...,HYPn4)
  skip;
  nskip `ExprID';
  multiply replace_(HYPtoDiff, `FuncID');
  id `FuncID' = 1;
  #call HypSimplifyGeneralDiffLogDiffFuncs(`ExprID',HYPdiff,`RatFuncID',`PolyFuncID',HYPn1,...,HYPn8)
  #call HypSimplifyDiffLogDiffFuncs(`ExprID',HYPdiff,`RatFuncID',`PolyFuncID',HYPaux1,...,HYPaux4,`Var',HYPn1,...,HYPn10)
  #call HypDiffLogDiffs(`ExprID',HYPdiff,dum_,dum_,HYPdenx,HYPn1,...,HYPn4)
  #call HypSimplifyVarDepDens(`ExprID',HYPdenx,HYPdenx,HYPn1,...,HYPn3)
  #if (`IsolateForZeroLimit')
    #if (`FibrationBasisZeroLimitBeforeDiff')
      #call HypEliminateVarDepPolysForIndepFuncs(`FuncID',`RatFuncID',`PolyFuncID',HYPaux,HYPn1,HYPn2)
      #call HypEliminateRatFuncArgsForNumericArgs(`FuncID',`RatFuncID',`PolyFuncID',HYPaux,HYPn1,HYPn2)
      #call HypFromLimitHlogsToNumbers(`ExprID',`FuncID',HYPLinf)
    #endif
  #endif
  #if 0
  #endif
#endprocedure
#procedure HypIsolateAndDissectVarDepFunctions(ExprID,NewExprID,FuncID,AuxFuncID,RatFuncID,PolyFuncID,Var,Sym1,Sym2,MinArgLength,MaxArgLength)
  #call HypCreateFunctionExpressionWithExtras(`NewExprID',`FuncID',0,`ExprID')
  skip;
  nskip `NewExprID';
  #call HypArgsToRatFunc(`NewExprID', `FuncID', `RatFuncID', `AuxFuncID', `Sym1')
  #call HypDropIndepTerms(`NewExprID', `FuncID', `RatFuncID', `Var', HYPSol, 1, `PolyFuncID', `Sym1', `Sym2')
  skip;
  nskip `NewExprID';
  dropcoefficient;
  #call HypDissectExprWithArgNumber(`NewExprID', `NewExprID', `FuncID', `AuxFuncID', `Sym1',`MinArgLength',`MaxArgLength',1,0)
  #if (`HYPdebug')
    #message Minimum arg length is ``MinArgLength'';
    #message Maximum arg length is ``MaxArgLength'';
  #endif
#endprocedure
#procedure HypCollectHlogsForFibrationBasis(ExprID,SolutionExprID,FuncID,Var,MinArgLength,MaxArgLength)
  .sort:CollectHlogsForFibrationBasis start;
  #call HypIsolateAndDissectVarDepFunctionsThruExtra(`SolutionExprID',`FuncID',HYPmatch,HYPrat,HYPpoly,`Var',HYPn,1,`HYPMAXMATCH',`MinArgLength',`MaxArgLength',`ExprID')
  #do iLevel=``MinArgLength'',``MaxArgLength''
    #call HypCreateRatFuncArgsInVar(`SolutionExprID'`iLevel',`FuncID',0,,HYPrat,HYPpoly,`Var',HYPaux1,HYPn1,HYPn2)
  #enddo
  .sort:Created detailed ratfunc args;
  #if (``MaxArgLength'' > 0)
    skip;
    nskip <`SolutionExprID'``MinArgLength''>,...,<`SolutionExprID'``MaxArgLength''>;
    id `FuncID'(?a) = `FuncID'(?a)*HYPsol(?a);
  #endif
  .sort:CollectHlogsForFibrationBasis fin;
#endprocedure
#procedure HypRegularizeAndDifferentiateForFibrationBases(SolutionExprID,FuncID,Var,MinWordLen,MaxWordLen)
  .sort:RegularizeAndDifferentiateForFibrationBases start;
  #do iLevel=``MaxWordLen'',1,-1
    #if (exists(`SolutionExprID'`iLevel') == 0)
      local `SolutionExprID'`iLevel' = 0;
      .sort:`SolutionExprID'`iLevel' missing, created;
    #endif
  #enddo
  #do iLevel=``MaxWordLen'',1,-1
    skip;
    nskip `SolutionExprID'`iLevel';
    if (count(HYPsol, 1) == 0) id `FuncID'(?a) = `FuncID'(?a)*HYPauxSol(?a);
    .sort:New solutions labeled with HYPauxSol;
    skip;
    nskip `SolutionExprID'`iLevel';
    abracket HYPsol,HYPauxSol;
    .sort:Bracketed HYPsol,HYPauxSol;
    skip;
    nskip `SolutionExprID'`iLevel';
    collect HYPacc;
    splitarg HYPacc;
    id HYPacc(?a,HYPsol(?b),?c) = HYPsol(?b);
    id HYPacc(HYPn1?) = HYPn1;
    if (count(HYPacc,1) > 0);
      print "Weird HYPacc presence: %t";
      print "RegularizeAndDifferentiateForFibrationBases";
      exit;
    endif;
    .sort:Duplicate solutions dropped;
    #call HypShuffleRegularizeAtZero(`SolutionExprID'`iLevel',`FuncID',HYPrat,HYPpoly,1,HYPsol,HYPn1,HYPn2,HYPaux1,...,HYPaux3)
    .sort:Shuffled to reg at zero;
    skip;
    nskip `SolutionExprID'`iLevel';
    #call HypDifferentiateHlogsAtLevel(`SolutionExprID'`iLevel',`FuncID',HYPrat,HYPpoly,`Var',`iLevel',0,,1,HYPLzero,HYPLconstZeroLimit)
    .sort:Diff at level `iLevel' done;
    #define MinRemnantWordLength ""
    #define MaxRemnantWordLength ""
    #call HypIsolateAndDissectVarDepFunctions(`SolutionExprID'`iLevel',`SolutionExprID'TMP`iLevel',`FuncID',HYPaux,HYPrat,HYPpoly,`Var',HYPn1,HYPn2,MinRemnantWordLength,MaxRemnantWordLength)
    #call HypMergeLevelExpressions(`SolutionExprID',`SolutionExprID'TMP`iLevel',1,{`iLevel'-1},1)
  #enddo
  .sort:RegularizeAndDifferentiateForFibrationBases fin;
#endprocedure
#procedure HypDetermineZeroLimitsForFibrationBasis(SolutionExprID,FuncID,Var,MinWordLen,MaxWordLen)
  .sort:DetermineZeroLimitsForFibrationBasis start;
  #define MinZeroLimitWordLength ""
  #define MaxZeroLimitWordLength ""
  #call HypIsolateAndDissectVarDepFunctionsThruExtra(IntVar`CURRENTINTVAR'ZeroLimit,HYPLzero,HYPmatch,HYPrat,HYPpoly,`Var',HYPn,1,`MaxWordLen',MinZeroLimitWordLength,MaxZeroLimitWordLength,<`SolutionExprID'`MinWordLen'>,...,<`SolutionExprID'`MaxWordLen'>)
  skip;
  nskip <IntVar`CURRENTINTVAR'ZeroLimit`MinZeroLimitWordLength'>,...,<IntVar`CURRENTINTVAR'ZeroLimit`MaxZeroLimitWordLength'>;
  dropcoefficient;
  #if (`ToNumberInFibrationBasisZeroLimit')
    id HYPLzero(?a) = 
      (
        HYPLzero(?a)
        +
        HYPzero
      )*HYPsol(?a)
    ;
  #else
    id HYPLzero(?a) = HYPLzero(?a)*HYPsol(?a);
  #endif
  .sort:Solution functs introduced;
  #do iLevel=`MinZeroLimitWordLength',`MaxZeroLimitWordLength'
    skip;
    nskip IntVar`CURRENTINTVAR'ZeroLimit`iLevel';
    #call HypTakeZeroLimitInLimitHlog(IntVar`CURRENTINTVAR'ZeroLimit`iLevel',HYPLzero,HYPrat,HYPpoly,HYPcrat,HYPaux1,...,HYPaux3,`FuncID',HYPn1,...,HYPn3)
  #enddo
  .sort:Taking zero limit is over;
  skip;
  nskip <IntVar`CURRENTINTVAR'ZeroLimit`MinZeroLimitWordLength'>,...,<IntVar`CURRENTINTVAR'ZeroLimit`MaxZeroLimitWordLength'>;
  #call HypSimplifyRatFuncsTrivial(`FuncID',HYPcrat,HYPcrat,HYPpoly,HYPn1,HYPn2)
  #do ilevel=`MinZeroLimitWordLength',`MaxZeroLimitWordLength'
    #call HypSimplifyRatFuncArguments(IntVar`CURRENTINTVAR'ZeroLimit`ilevel', `FuncID', HYPcrat, HYPaux, HYPdiv1, HYPdiv2, HYPgcd, HYPsimpTbl, HYPn1, HYPn2, HYPn3)
    #if (`ToNumberInFibrationBasisZeroLimit')
      #call HypFromLimitHlogsToNumbers(IntVar`CURRENTINTVAR'ZeroLimit`ilevel',`FuncID',HYPLinf)
    #endif
  #enddo
  skip;
  nskip <IntVar`CURRENTINTVAR'ZeroLimit`MinZeroLimitWordLength'>,...,<IntVar`CURRENTINTVAR'ZeroLimit`MaxZeroLimitWordLength'>;
  multiply replace_(HYPcrat, HYPrat);
  .sort:Simplifications of zero limit over;
  #do iLevel=`MinZeroLimitWordLength',`MaxZeroLimitWordLength'
    skip;
    #call HypSubstituteToExprsWithExtrasAndTable(IntVar`CURRENTINTVAR'ZeroLimit`iLevel',HYPLzero,HYPsol,HYPaux,HYPsolutionTbl,<`SolutionExprID'`iLevel'>,...,<`SolutionExprID'`MaxWordLen'>)
    #if (`ToNumberInFibrationBasisZeroLimit')
      skip;
      nskip <`SolutionExprID'`iLevel'>,...,<`SolutionExprID'`MaxWordLen'>;
      id HYPzero = 0;
    #endif
    .sort:level `iLevel' finished;
    skip;
    drop IntVar`CURRENTINTVAR'ZeroLimit`iLevel';
    .sort:ZeroLimit`iLevel' dropped;
  #enddo
  .sort:DetermineZeroLimitsForFibrationBasis fin;
#endprocedure
#procedure HypLimitsToHlogs(ExprID,FuncID,Var)
  .sort:LimitsToHlogsDev start;
  #redefine CURRENTINTVAR "{`CURRENTINTVAR'+1}"
  #call HypSimplifyRatFuncArguments(`ExprID', `FuncID', HYPrat, HYPaux1, HYPaux2, HYPaux3, HYPaux4, HYPsimpTbl, HYPn1, HYPn2, HYPn3)
  .sort:LimitsToHlogsDev fin;
  #define MinWordLength ""
  #define MaxWordLength ""
  #call HypCollectHlogsForFibrationBasis(`ExprID',IntVar`CURRENTINTVAR'Sols,`FuncID',`Var',MinWordLength,MaxWordLength)
  hide `ExprID';
  .sort:Original `ExprID' hidden;
  #call HypRegularizeAndDifferentiateForFibrationBases(IntVar`CURRENTINTVAR'Sols,`FuncID',`Var',MinWordLength,MaxWordLength)
  #call HypIncludeTagInExprsWhenFunctionPresent(HYPdx,HYPdenx,<IntVar`CURRENTINTVAR'Sols1>,...,<IntVar`CURRENTINTVAR'Sols`MaxWordLength'>)
  #call HypIntegrateAtLevelSubstituteAbove(IntVar`CURRENTINTVAR'Sols,1,`MaxWordLength',HYPLx,`FuncID',HYPdenx,HYPrat,HYPaux,HYPdx,HYPn1,HYPn2,HYPsol,HYPauxSol)
  #if ((`MaxWordLength' > 0) && (`MinWordLength' > 1))
      drop <IntVar`CURRENTINTVAR'Sols1>,...,<IntVar`CURRENTINTVAR'Sols{`MinWordLength'-1}>;
      .sort: <IntVar`CURRENTINTVAR'Sols1>,...,<IntVar`CURRENTINTVAR'Sols{`MinWordLength'-1}> dropped;
  #endif
  #if (`MaxWordLength' > 0)
    .sort
    skip;
    nskip <IntVar`CURRENTINTVAR'Sols`MinWordLength'>,...,<IntVar`CURRENTINTVAR'Sols`MaxWordLength'>;
    if (count(HYPauxSol,1) > 0) discard;
    .sort auxiliary sols are purged;
  #endif
  .sort:Convert detailed rat funcs;
  skip;
  #do func={HYPsol,HYPLx,`FuncID'}
    #if (`HYPdebug')
      #message Converting back `func';
    #endif
    #call HypConvertDetailedRatFuncs(`func',HYPrat,HYPpoly,`Var',HYPn1,HYPn2,HYPn3,<IntVar`CURRENTINTVAR'Sols`MinWordLength'>,...,<IntVar`CURRENTINTVAR'Sols`MaxWordLength'>)
  #enddo
  #if (`FibrationBasisZeroLimitBeforeDiff' == 0)
    #call HypDetermineZeroLimitsForFibrationBasis(IntVar`CURRENTINTVAR'Sols,`FuncID',`Var',`MinWordLength',`MaxWordLength')
    .sort:Zero limits substituted;
  #else
    InExpression <IntVar`CURRENTINTVAR'Sols`MinWordLength'>,...,<IntVar`CURRENTINTVAR'Sols`MaxWordLength'>;
      multiply replace_(HYPLconstZeroLimit,`FuncID');
    EndInExpression;
    .sort:Before substitution;
  #endif
  unhide `ExprID';
  .sort:`ExprID' unhidden;
  #do iLevel=`MinWordLength',`MaxWordLength'
    #if (`HYPdebug')
      #message SUBSTITUTION AT LEVEL `iLevel';
    #endif
    #call HypSubstituteToExprsWithExtrasAndTable(IntVar`CURRENTINTVAR'Sols`iLevel',`FuncID',HYPsol,HYPaux,HYPsolutionTbl,`ExprID')
  #enddo
  .sort:Substs done;
  drop <IntVar`CURRENTINTVAR'Sols`MinWordLength'>,...,<IntVar`CURRENTINTVAR'Sols`MaxWordLength'>;
  #if 0
  #endif
  .sort:Sols purged;
#endprocedure
#procedure HypDissectArg(FuncID,Func1ID,Func2ID)
  id `FuncID'(?a) = `FuncID'(?a,HYPsep);
  #do i=1,1
    id `FuncID'(?a,HYPn1?,HYPsep,?b) = `FuncID'(?a,HYPsep,HYPn1,?b) +
      `Func1ID'(?a,HYPn1)*`Func2ID'(?b);
    id `FuncID'(HYPsep,?a) = `Func1ID'*`Func2ID'(?a);
    if (match(`FuncID'(?a,HYPsep,?b)) > 0) redefine i "0";
    .sort:DissectArg;
  #enddo
#endprocedure
#procedure HypRevealVarFromArg(FuncID, AuxFuncID, Var)
  .sort:RevealVarFromArg starts;
  bracket `FuncID';
  .sort:Bracketed `FuncID';
  keep brackets;
  splitarg((`Var')),`FuncID';
  id `FuncID'(`Var') = `Var';
  transform,`FuncID',addargs(2,last);
  repeat;
    id,once `FuncID'(HYPn1?, HYPn2?) = `FuncID'(HYPn1)
                               + `AuxFuncID'(HYPn2/`Var')*`Var';
      repeat;
        if (count(`AuxFuncID',1) > 0);
          id `AuxFuncID'(HYPn1?) = `FuncID'(rem_(HYPn1,`Var'))
                              + `AuxFuncID'(div_(HYPn1,`Var'))*`Var';
        endif;
        id `FuncID'?{`FuncID', `AuxFuncID'}(0) = 0;
      endrepeat;
    endrepeat;
  .sort:RevealVarFromArg fin;
#endprocedure
#procedure HypRevealVarDenominator(Var)
  id HYPden(`Var') = HYPdenx(0);
  splitarg((`Var')),HYPden;
  id HYPden(HYPn1?,0) = HYPden(HYPn1);
  transform,HYPden,addargs(2,last);
  argument HYPden,2;
    multiply 1/`Var';
  endargument;
  id HYPden(HYPn1?,HYPn2?number_) = 1/HYPn2*HYPdenx(-HYPn1/HYPn2);
  repeat;
    id,once HYPden(HYPn1?,HYPn2?) = HYPden(HYPn2)*HYPaux(-HYPn1,HYPn2);
    id HYPaux(HYPn1?,HYPn2?) = HYPaux(gcd_(HYPn1,HYPn2),HYPn1,HYPn2);
    id HYPaux(1,HYPn1?,HYPn2?) = HYPdenx(HYPn1,HYPn2);
    id HYPaux(HYPn3?,HYPn1?,HYPn2?) = HYPdenx(div_(HYPn1,HYPn3),div_(HYPn2,HYPn3));
  endrepeat;
  id HYPdenx(HYPn1?,HYPn2?number_) = HYPdenx(HYPn1/HYPn2);
#endprocedure
#procedure HypRevealVarLog(Var)
  id HYPlog(HYPn1?) = HYPaux(HYPlog(HYPn1));
  Argument HYPaux;
    If (occurs(`Var') > 0);
      multiply replace_(HYPlog,HYPaux);
    EndIf;
  EndArgument;
  id HYPaux(HYPaux(HYPn1?)) = HYPaux(HYPn1);
  Factarg HYPaux;
  Repeat id HYPaux(HYPn1?,HYPn2?,?a) = HYPlog(HYPn1) + HYPaux(HYPn2,?a);
  id HYPaux(HYPlog(HYPn1?)) = HYPlog(HYPn1);
  id HYPaux(HYPn1?) = HYPlog(HYPn1);
  id HYPlog(`Var') = HYPlogx(0);
  splitarg((`Var')),HYPlog;
  transform,HYPlog,addargs(2,last);
  id HYPlog(HYPn1?,HYPn2?!{0,}) = HYPlog(HYPn1) + HYPlogx(-HYPn1,HYPn2/`Var');
  id HYPlog(HYPn1?,0) = HYPlog(HYPn1);
  id HYPlogx(HYPn1?,HYPn2?number_) = HYPlogx(HYPn1/HYPn2);
#endprocedure
#procedure HypRevealVar(Var)
  id HYPpow(`Var',HYPn1?{>0}) = `Var'^HYPn1;
  id HYPpow(`Var',HYPn1?{<0}) = HYPdenx(0)^(-HYPn1);
  #call HypRevealVarFromArg(HYPnum, HYPaux, `Var')
  #call HypRevealVarDenominator(`Var')
  #call HypRevealVarLog(`Var')
  id HYPL(HYPw(?a),`Var') = HYPLx(?a);
  #if 0
  #endif
#endprocedure
#procedure HypChangeFuncHeadForSameArgFuncs(InFuncID,SameArgFuncID,Sym1,Sym2,Arg)
  id `InFuncID'(?a,`Arg',?b) = `SameArgFuncID'(?a,`Arg',?b);
  id `SameArgFuncID'(?a,`Sym1'?!{`Sym2'?},`Sym2'?!{`Sym1'?},?b) = `InFuncID'(?a,`Sym1',`Sym2',?b);
#endprocedure
#procedure HypFromLinfToLone(ExprID,LinfFuncID,LoneFuncID,AuxFuncID,Aux2FuncID,Sym)
  .sort:FromLinfToLone start;
  skip;
  nskip `ExprID';
  repeat;
    id,once `LinfFuncID'(?a) = `AuxFuncID'(?a);
    if (count(`AuxFuncID',1) > 0);
      multiply `Aux2FuncID';
      repeat;
        id `AuxFuncID' = 1;
        id `Aux2FuncID'(?a)*`AuxFuncID'(`Sym'?!{-1,},?b) = 
          (
            `Aux2FuncID'(?a,`Sym'/(1 + `Sym'))
            -
            `Aux2FuncID'(?a,1)
          )*`AuxFuncID'(?b)
        ;
        also `Aux2FuncID'(?a)*`AuxFuncID'(-1,?b) = 
          - `Aux2FuncID'(?a,1)*`AuxFuncID'(?b)
        ;
      endrepeat;
      multiply replace_(`Aux2FuncID',`LoneFuncID');
    endif;
  endrepeat;
  .sort:FromLinfToLone fin;
#endprocedure
#procedure HypFromLoneToHarmonic(ExprID,LoneFuncID,HfuncID,AuxFuncID,Aux2FuncID,Aux3FuncID,Sym1,Sym2)
  .sort:FromLoneToHarmonic start;
  skip;
  nskip `ExprID';
  repeat;
    if (count(`LoneFuncID',1) > 0);
      id,once `LoneFuncID'(?a) = `AuxFuncID'(?a);
      id `AuxFuncID'(?a) = `AuxFuncID'(?a)*`Aux2FuncID'(?a);
      transform,`Aux2FuncID',addargs(1,last);
      id `Aux2FuncID'(`Sym1'?) = (-1)^`Sym1';
      id `AuxFuncID'(?a) = `AuxFuncID'(?a)*`Aux2FuncID'(-nargs_(?a)/2,?a);
      transform,`Aux2FuncID',addargs(1,last);
      id `Aux2FuncID'(`Sym1'?) = `Aux2FuncID'(2*`Sym1');
      if (match(`Aux2FuncID'(`Sym1'?{0,})) > 0);
        id `AuxFuncID'(?a)*`Aux2FuncID'(`Sym1'?) = 
          `AuxFuncID'(?a)*`Aux2FuncID'(?a)*`Aux3FuncID'(nargs_(?a));
        transform,`Aux2FuncID',replace(1,last)=(0,1,1,0),reverse(1,last);
        #do ToDecimalFunc={`AuxFuncID',`Aux2FuncID'}
          transform,'ToDecimalFunc',encode(1,last):base=2;
        #enddo
        id `Aux3FuncID'(`Sym1'?$nargs) = 1;
        id `AuxFuncID'(`Sym1'?)*`Aux2FuncID'(`Sym2'?) = 
          `Aux3FuncID'(`Sym1'-`Sym2',`Sym1',`Sym2');
        id `Aux3FuncID'(`Sym1'?pos0_,?a,`Sym2'?) =
          `Aux3FuncID'(`Sym2')
        ;
        also `Aux3FuncID'(`Sym1'?neg_,`Sym2'?,?a) =
          `Aux3FuncID'(`Sym2')
        ;
        transform,`Aux3FuncID',decode(1,$nargs):base=2;
      elseif (match(`Aux2FuncID'(`Sym1'?pos_)) > 0);
        id `Aux2FuncID'(`Sym1'?) = 1;
        transform,`AuxFuncID',replace(1,last)=(0,1,1,0),reverse(1,last);
        multiply replace_(`AuxFuncID',`Aux3FuncID');
      elseif (match(`Aux2FuncID'(`Sym1'?neg_)) > 0);
        id `Aux2FuncID'(`Sym1'?) = 1;
        multiply replace_(`AuxFuncID',`Aux3FuncID');
      endif;
      multiply replace_(`Aux3FuncID',`HfuncID');
    endif;
  endrepeat;
  ModuleOption,local,$nargs;
  .sort:FromLoneToHarmonic fin;
#endprocedure
#procedure HypFromHarmonicToMZV(ExprID,HfuncID,MZVfuncID,SymHead,MaxWordLength)
  .sort:FromHarmonicToMZV start;
  #ifndef `INITMZV'
    #write "INITMZV preprocessor variable is missing..."
    #write "FromHarmonicToMZV"
    #terminate 1;
  #else 
    #if (`INITMZV' == 1)
      #if (`MaxWordLength' > `MAXMZVWORDLENGTH')
        #redefine MAXMZVWORDLENGTH "`MaxWordLength'"
      #endif
      #call readmzvs(`MAXMZVWORDLENGTH')
      #redefine INITMZV "0"
      .sort:mzvlow inited, FromHarmonicToMZV;
    #endif
  #endif
  skip;
  nskip `ExprID';
  #do WordLength=1,`MaxWordLength'
    id `HfuncID'(<`SymHead'1?>,...,<`SymHead'`WordLength'?>) =
      `MZVfuncID'`WordLength'(<`SymHead'1>,...,<`SymHead'`WordLength'>)
    ;
  #enddo
  .sort:FromHarmonicToMZV fin;
#endprocedure
#procedure HypFromLimitHlogsToNumbers(ExprID,RegLinfFuncID,LinfFuncID)
  .sort:FromLimitHlogsToNumbers start;
  skip;
  nskip `ExprID';
  argument `RegLinfFuncID';
    #call HypToNumbers()
  endargument;
  multiply replace_(`RegLinfFuncID',HYPaux);
  id HYPaux(?a,HYPn1?!number_,?b) = `RegLinfFuncID'(?a,HYPn1,?b);
  #call HypChangeFuncHeadForSameArgFuncs(HYPaux,HYPaux2,HYPn1,HYPn2,0)
  #call HypChangeFuncHeadForSameArgFuncs(HYPaux,HYPaux2,HYPn1,HYPn2,-1)
  id HYPaux2(?a) = 0;
  id HYPaux(?a,HYPn1?!number_,?b) = `RegLinfFuncID'(?a,HYPn1,?b);
  id HYPaux(?a,HYPn1?!{0,-1},?b) = `RegLinfFuncID'(?a,HYPn1,?b);
  #call HypShuffleRegularizeAtZero(`ExprID',HYPaux,HYPrat,HYPpoly,0,,HYPn1,HYPn2,HYPaux1,...,HYPaux3)
  #call HypChangeFuncHeadForSameArgFuncs(HYPaux,HYPaux2,HYPn1,HYPn2,0)
  id HYPaux2(?a) = 0;
  #call HypShuffleRegularizeAtInfinity(`ExprID',HYPaux,0,,HYPn1,HYPn2,HYPaux1,...,HYPaux4)
  multiply replace_(HYPaux,`LinfFuncID');
  shuffle `LinfFuncID';
  #call HypFromLinfToLone(`ExprID',`LinfFuncID',HYPLone,HYPaux,HYPaux2,HYPn1)
  #call HypFromLoneToHarmonic(`ExprID',HYPLone,HYPH,HYPaux,HYPaux2,HYPaux3,HYPn1,HYPn2)
  #call HypFromHarmonicToMZV(`ExprID',HYPH,mzv,HYPn,`MAXMZVWORDLENGTH')
  .sort:FromLimitHlogsToNumbers fin;
#endprocedure
#procedure HypToHyperlog()
  id HYPlogx(HYPn1?,HYPn2?) = HYPlogx(gcd_(HYPn1,HYPn2),HYPn1,HYPn2);
  id HYPlogx(1,HYPn1?,HYPn2?) = HYPlogx(HYPrat(HYPn1,HYPn2)); 
  id HYPlogx(HYPn3?,HYPn1?,HYPn2?) = HYPlogx(HYPrat(div_(HYPn1,HYPn3),div_(HYPn2,HYPn3))); 
  multiply replace_(HYPlogx,HYPLx);
  shuffle,HYPLx;
#endprocedure
#procedure HypDecomposeWRTvar(Var)
  repeat id `Var'*HYPdenx(0) = 1;
  #do i=1,1
    #$it = 1;
    if (match(`Var'*HYPdenx(HYPn1?$poly)) > 0);
      FactDollar $poly;
      $FactPoly = 1;
      do $i=1,$poly[0];
        $FactPoly = $FactPoly*HYPnum($poly[$i]);
      enddo;
      id,once `Var'*HYPdenx($poly) = 1 + $FactPoly*HYPdenx($poly);
      #call HypSimplifyNumsDens(HYPnum,HYPden,HYPn1)
    endif;
    if ((count(`Var',1) > 0) && (match(HYPdenx(HYPn1?)) > 0)) $it = 0;
    ModuleOption,minimum,$it;
    ModuleOption,local,$poly,$FactPoly,$i;
    .sort:Cancel towers simple `Var';
    #redefine i "`$it'"
  #enddo
  #do i=1,1
    #$it = 1;
    if (match(`Var'*HYPdenx(HYPn1?$poly1,HYPn2?$poly2)) > 0);
      FactDollar $poly1;
      $FactPoly1 = 1;
      do $i=1,$poly1[0];
        $FactPoly1 = $FactPoly1*HYPnum($poly1[$i]);
      enddo;
      FactDollar $poly2;
      $FactPoly2 = 1;
      do $i=1,$poly2[0];
        $FactPoly2 = $FactPoly2*HYPden($poly2[$i]);
      enddo;
      id,once `Var'*HYPdenx($poly1,$poly2) = 1 + $FactPoly1*$FactPoly2*HYPdenx($poly1,$poly2);
      #call HypSimplifyNumsDens(HYPnum,HYPden,HYPn1)
    endif;
    if ((count(`Var',1) > 0) && (match(HYPdenx(HYPn1?,HYPn2?)) > 0)) $it = 0;
    ModuleOption,minimum,$it;
    ModuleOption,local,$poly1,$poly2,$FactPoly1,$FactPoly2,$i;
    .sort:Cancel towers ratfun `Var';
    #redefine i "`$it'"
  #enddo
  #do i=1,1
    #$it = 1;
    id,once HYPdenx(HYPn1?!{HYPn2?})*HYPdenx(HYPn2?!{HYPn1?}) = 
      HYPdiv1(HYPn1)*HYPdiv2(HYPn2)*HYPgcd(gcd_(HYPn1, HYPn2))*(HYPdenx(HYPn1) - HYPdenx(HYPn2));
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd(HYPn3?!number_) =
      HYPden(HYPn3)*HYPdiv1(div_(HYPn1, HYPn3))*HYPdiv2(div_(HYPn2, HYPn3));
    ;
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd(HYPn3?number_) =
      1/HYPn3*HYPdiv1(HYPn1/HYPn3)*HYPdiv2(HYPn2/HYPn3);
    ;
    id HYPdiv1(HYPn1?number_)*HYPdiv2(HYPn2?number_) = 1/(HYPn1-HYPn2);
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?) = HYPden(HYPn1 - HYPn2);
    if (match(HYPdenx(HYPn1?!{HYPn2?})*HYPdenx(HYPn2?!{HYPn1?})) > 0) $it = 0;
    ModuleOption,minimum,$it;
    .sort:Partial fraction simple;
    #redefine i "`$it'"
  #enddo
  #do i=1,1
    #$it = 1;
    id,once HYPdenx(HYPn1?)*HYPdenx(HYPn2?,HYPn3?) = 
      HYPnum(HYPn3)*
      HYPdiv1(HYPn1)*HYPdiv2(HYPn2, HYPn3)*HYPgcd(gcd_(HYPn1, HYPn2))*
      (HYPdenx(HYPn1) - HYPdenx(HYPn2,HYPn3));
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?, HYPn3?)*HYPgcd(HYPn4?!number_) =
      HYPden(HYPn4)*HYPdiv1(div_(HYPn1, HYPn4))*HYPdiv2(div_(HYPn2, HYPn4), HYPn3);
    ;
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?, HYPn3?)*HYPgcd(HYPn4?number_) =
      1/HYPn4*HYPdiv1(HYPn1/HYPn4)*HYPdiv2(HYPn2/HYPn4, HYPn3);
    ;
    if ((count(HYPdiv1,1) > 0) &&
        (count(HYPdiv2,1) > 0));
      id HYPdiv1(HYPn1?$n1)*HYPdiv2(HYPn2?$n2, HYPn3?$n3) = 1;
      $mul = mul_($n1, $n3) - $n2;
      factdollar $mul;
      do $i=1, $mul[0];
        multiply HYPden($mul[$i]);
      enddo;
    endif;
    if (match(HYPdenx(HYPn1?)*HYPdenx(HYPn2?,HYPn3?)) > 0) $it = 0;
    ModuleOption,minimum,$it;
    ModuleOption,local,$n1, $n2, $n3, $i, $mul;
    .sort:Partial fraction ratfun;
    #redefine i "`$it'"
  #enddo
  #do i=1,1
    #$it = 1;
    id,once HYPdenx(HYPn1?,HYPn2?!{HYPn4})*HYPdenx(HYPn1?,HYPn4?!{HYPn2?}) = 
      HYPdiv1(HYPn2)*HYPdiv2(HYPn4)*HYPgcd(gcd_(HYPn2, HYPn4))*
      HYPden(HYPn1)*
      (HYPdenx(HYPn1,HYPn2) - HYPdenx(HYPn1,HYPn4))
    ;
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd(HYPn3?!number_) =
      HYPnum(HYPn3)*HYPdiv1(div_(HYPn1, HYPn3))*HYPdiv2(div_(HYPn2, HYPn3));
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd(HYPn3?number_) =
      HYPn3*HYPdiv1(HYPn1/HYPn3)*HYPdiv2(HYPn2/HYPn3);
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?) =
      HYPnum(HYPn1)*HYPnum(HYPn2)*HYPden(HYPn2 - HYPn1);
    if (match(HYPdenx(HYPn1?,HYPn2?!{HYPn4})*
              HYPdenx(HYPn1?,HYPn4?!{HYPn2?})) > 0) $it = 0;
    ModuleOption,minimum,$it;
    .sort:Partial fraction ratfun2;
    #redefine i "`$it'"
  #enddo
  #do i=1,1
    #$it = 1;
    id,once HYPdenx(HYPn1?!{HYPn3?},HYPn2?)*HYPdenx(HYPn3?!{HYPn1?},HYPn2?) = 
      HYPnum(HYPn2)*
      HYPdiv1(HYPn1)*HYPdiv2(HYPn3)*HYPgcd(gcd_(HYPn1, HYPn3))*
      (HYPdenx(HYPn1,HYPn2) - HYPdenx(HYPn3,HYPn2))
    ;
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd(HYPn3?!number_) =
      HYPden(HYPn3)*HYPdiv1(div_(HYPn1, HYPn3))*HYPdiv2(div_(HYPn2, HYPn3));
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd(HYPn3?number_) =
      1/HYPn3*HYPdiv1(HYPn1/HYPn3)*HYPdiv2(HYPn2/HYPn3);
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?) = HYPden(HYPn1 - HYPn2);
    if (match(HYPdenx(HYPn1?!{HYPn3?},HYPn2?)*
              HYPdenx(HYPn3?!{HYPn1?},HYPn2?)) > 0) $it = 0;
    ModuleOption,minimum,$it;
    .sort:Partial fraction ratfun3;
    #redefine i "`$it'"
  #enddo
  #do i=1,1
    #$it = 1;
    id,once HYPdenx(HYPn1?!{HYPn3?},HYPn2?!{HYPn4?})*HYPdenx(HYPn3?!{HYPn1?},HYPn4?!{HYPn2?}) = 
      HYPdiv1(HYPn1)*HYPdiv2(HYPn3)*HYPgcd1(gcd_(HYPn1, HYPn3))*
      HYPdiv3(HYPn2)*HYPdiv4(HYPn4)*HYPgcd2(gcd_(HYPn2, HYPn4))*
      (HYPdenx(HYPn1,HYPn2) - HYPdenx(HYPn3,HYPn4))
    ;
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd1(HYPn3?!number_) =
      HYPden(HYPn3)*HYPdiv1(div_(HYPn1, HYPn3))*HYPdiv2(div_(HYPn2, HYPn3));
    id HYPdiv3(HYPn1?)*HYPdiv4(HYPn2?)*HYPgcd2(HYPn3?!number_) =
      HYPnum(HYPn3)*HYPdiv3(div_(HYPn1, HYPn3))*HYPdiv4(div_(HYPn2, HYPn3));
    id HYPdiv1(HYPn1?)*HYPdiv2(HYPn2?)*HYPgcd1(HYPn3?number_) =
      1/HYPn3*HYPdiv1(HYPn1/HYPn3)*HYPdiv2(HYPn2/HYPn3);
    id HYPdiv3(HYPn1?)*HYPdiv4(HYPn2?)*HYPgcd2(HYPn3?number_) =
      HYPn3*HYPdiv3(HYPn1/HYPn3)*HYPdiv4(HYPn2/HYPn3);
    if ((count(HYPdiv1,1) > 0) &&
        (count(HYPdiv2,1) > 0) &&
        (count(HYPdiv3,1) > 0) &&
        (count(HYPdiv4,1) > 0));
      id HYPdiv1(HYPn1?$n1)*HYPdiv2(HYPn3?$n3)*HYPdiv3(HYPn2?$n2)*HYPdiv4(HYPn4?$n4) =
        HYPnum($n2)*HYPnum($n4)
      ;
      $mul = mul_($n1, $n4) - mul_($n3, $n2);
      factdollar $mul;
      do $i=1, $mul[0];
        multiply HYPden($mul[$i]);
      enddo;
    endif;
    if (match(HYPdenx(HYPn1?!{HYPn3?},HYPn2?!{HYPn4?})*
              HYPdenx(HYPn3?!{HYPn1?},HYPn4?!{HYPn2?})) > 0) $it = 0;
    ModuleOption,local,$n1, $n2, $n3, $n4, $i, $mul;
    ModuleOption,minimum,$it;
    .sort:Partial fraction ratfun4;
    #redefine i "`$it'"
  #enddo
#endprocedure
#procedure HypMoebiusTransform(FuncID,Op)
  id `FuncID'(?a) = `FuncID'(HYPPhi(?a));
  repeat id `FuncID'(HYPPhi(?a,HYPn1?,HYPn2?),?b) = `FuncID'(HYPPhi(?a,HYPn1),HYPPhi(HYPn2),?b);
  #do i=1,1
    #if (`Op' == "1/z")
      if (match(`FuncID'(?a,HYPPhi(HYPrat(HYPn1?,HYPn2?)),?b)));
        id `FuncID'(?a,HYPPhi(HYPrat(HYPn1?,HYPn2?)),?b) = `FuncID'(?a,HYPrat(HYPn2,HYPn1),?b) - `FuncID'(?a,0,?b);
      else;
        id `FuncID'(?a,HYPPhi(HYPn1?),?b) = `FuncID'(?a,HYPden(HYPn1),?b) - `FuncID'(?a,0,?b);
      endif;
    #elseif (`Op' == "z/(1+z)")
      id `FuncID'(?a,HYPPhi(HYPn1?),?b) = `FuncID'(?a,HYPnum(HYPn1)*HYPden(1 + HYPn1),?b) - `FuncID'(?a,1,?b);
      id `FuncID'(?a,HYPnum(HYPn1?)*HYPden(0),?b) = 0;
    #endif
    id `FuncID'(?a,HYPden(0),?b) = 0;
    argument `FuncID';
      #call HypSimplifyNumerics
      #call HypRecastDenominators
    endargument;
    if (match(`FuncID'(?a,HYPPhi(HYPn1?),?b)) > 0) redefine i "0";
    .sort
  #enddo
#endprocedure
#procedure HypLimInfToLimZero(Var)
  if (match(HYPlim(`Var',HYPinf)) > 0);
    id HYPLx(?a,HYPn1?!{0,},?b) = HYPLlimInf(?a,HYPn1,?b);
    id HYPLx(?a) = (-1)^(nargs_(?a))*HYPLx(?a);
    id `Var' = HYPdenx(0);
  endif;
  #call HypDissectArg(HYPLlimInf,HYPLphi,HYPLinfRegInfZero)
  #call HypSimplifyNumerics
  #call HypMoebiusTransform(HYPLphi,"1/z")
  multiply replace_(HYPLphi,HYPLx);
  id HYPlim(`Var',HYPinf) = HYPlim(`Var',0);
#endprocedure
#procedure HypSubstituteGfunction(GfuncID,InvGfuncID,GammaFuncID,InvGammaFuncID,EpsSymbol,Sym1,...,Sym4)
  id `GfuncID'(<`Sym1'?>,...,<`Sym4'?>) =
    `GammaFuncID'(`Sym1'+`Sym2'-`Sym4'-2+`EpsSymbol')*
    `GammaFuncID'(-`Sym1'+`Sym3'-`Sym4'+2-`EpsSymbol')*
    `GammaFuncID'(-`Sym2'+`Sym4'+2-`EpsSymbol')*
    `InvGammaFuncID'(`Sym1')*
    `InvGammaFuncID'(`Sym2')*
    `InvGammaFuncID'(-`Sym1'-`Sym2'+`Sym3'+4-2*`EpsSymbol')
  ;
  id `InvGfuncID'(<`Sym1'?>,...,<`Sym4'?>) =
    `InvGammaFuncID'(`Sym1'+`Sym2'-`Sym4'-2+`EpsSymbol')*
    `InvGammaFuncID'(-`Sym1'+`Sym3'-`Sym4'+2-`EpsSymbol')*
    `InvGammaFuncID'(-`Sym2'+`Sym4'+2-`EpsSymbol')*
    `GammaFuncID'(`Sym1')*
    `GammaFuncID'(`Sym2')*
    `GammaFuncID'(-`Sym1'-`Sym2'+`Sym3'+4-2*`EpsSymbol')
  ;
#endprocedure
#procedure HypNormalizeGammaFunctions(EpsSymbol,GammaFuncID,InvGammaFuncID,NumFuncID,DenFuncID,Sym1,Sym2)
  splitarg,((`EpsSymbol')),`GammaFuncID';
  splitarg,((`EpsSymbol')),`InvGammaFuncID';
  id `GammaFuncID'(`Sym1'?pos_) = fac_(`Sym1'-1);
  id `InvGammaFuncID'(`Sym1'?pos_) = 1/fac_(`Sym1'-1);
  id `GammaFuncID'(`Sym1'?,`Sym2'?) = `GammaFuncID'(`Sym1',`Sym2'/`EpsSymbol');
  id `InvGammaFuncID'(`Sym1'?,`Sym2'?) = `InvGammaFuncID'(`Sym1',`Sym2'/`EpsSymbol');
  id `GammaFuncID'(`Sym1'?!number_) = `GammaFuncID'(0,`Sym1'/`EpsSymbol');
  id `InvGammaFuncID'(`Sym1'?!number_) = `InvGammaFuncID'(0,`Sym1'/`EpsSymbol');
  repeat id `GammaFuncID'(`Sym1'?pos_,`Sym2'?) = 
    `NumFuncID'(`Sym1'-1+`Sym2'*`EpsSymbol')*`GammaFuncID'(`Sym1'-1,`Sym2')
  ;
  repeat id `InvGammaFuncID'(`Sym1'?pos_,`Sym2'?) = 
    `DenFuncID'(`Sym1'-1+`Sym2'*`EpsSymbol')*`InvGammaFuncID'(`Sym1'-1,`Sym2')
  ;
  repeat id `GammaFuncID'(`Sym1'?neg_,`Sym2'?) = 
    `DenFuncID'(`Sym1'+`Sym2'*`EpsSymbol')*`GammaFuncID'(`Sym1'+1,`Sym2')
  ;
  repeat id `InvGammaFuncID'(`Sym1'?neg_,`Sym2'?) = 
    `NumFuncID'(`Sym1'+`Sym2'*`EpsSymbol')*`InvGammaFuncID'(`Sym1'+1,`Sym2')
  ;
  id `GammaFuncID'(`Sym1'?,`Sym2'?)*`InvGammaFuncID'(`Sym1'?,`Sym2'?) = 1;
#endprocedure
#procedure HypExpandGamma(GammaFuncID,InvGammaFuncID,EpsSymbol,Sym,MaxEpsPower)
  repeat;
    id,once `GammaFuncID'(0,`Sym'?) =
      1/(`Sym'*`EpsSymbol')
      -
      HYPEulerGamma
      +
      (6*HYPzeta2+6*HYPEulerGamma^2)/12*(`Sym'*`EpsSymbol')
      -
      (4*HYPzeta3+6*HYPEulerGamma*HYPzeta2+2*HYPEulerGamma^3)/12*(`Sym'*`EpsSymbol')^2
      +
      (160*HYPEulerGamma*HYPzeta3+108*HYPzeta2^2+120*HYPEulerGamma^2*HYPzeta2+20*HYPEulerGamma^4)/480*(`Sym'*`EpsSymbol')^3
      -
      (
        288*HYPzeta5+240*HYPzeta2*HYPzeta3+240*HYPEulerGamma^2*HYPzeta3+324*HYPEulerGamma*HYPzeta2^2
        +
        120*HYPEulerGamma^3*HYPzeta2+12*HYPEulerGamma^5
      )/1440*(`Sym'*`EpsSymbol')^4
      +
      (
        HYPEulerGamma*(24192*HYPzeta5+20160*HYPzeta2*HYPzeta3)+6720*HYPzeta3^2
        +
        6720*HYPEulerGamma^3*HYPzeta3+13176*HYPzeta2^3+13608*HYPEulerGamma^2*HYPzeta2^2
        +
        2520*HYPEulerGamma^4*HYPzeta2+168*HYPEulerGamma^6
      )/120960*(`Sym'*`EpsSymbol')^5
      +
      HYPGammaExpand(`EpsSymbol'^6)*`EpsSymbol'^6
    ;
    if (count(`EpsSymbol',1) > `MaxEpsPower') discard;
  endrepeat;
  repeat;
    id,once `InvGammaFuncID'(0,`Sym'?) =
      `Sym'*`EpsSymbol'
      +
      HYPEulerGamma*(`Sym'*`EpsSymbol')^2
      +
      (6*HYPEulerGamma^2-6*HYPzeta2)/12*(`Sym'*`EpsSymbol')^3
      +
      (4*HYPzeta3-6*HYPEulerGamma*HYPzeta2+2*HYPEulerGamma^3)/12*(`Sym'*`EpsSymbol')^4
      +
      (
        480*HYPEulerGamma*HYPzeta3+36*HYPzeta2^2-360*HYPEulerGamma^2*HYPzeta2+60*HYPEulerGamma^4
      )/1440*(`Sym'*`EpsSymbol')^5
      +
      (
        288*HYPzeta5-240*HYPzeta2*HYPzeta3+240*HYPEulerGamma^2*HYPzeta3+36*HYPEulerGamma*HYPzeta2^2
        -
        120*HYPEulerGamma^3*HYPzeta2+12*HYPEulerGamma^5
      )/1440*(`Sym'*`EpsSymbol')^6
      +
      (
        HYPEulerGamma*(24192*HYPzeta5-20160*HYPzeta2*HYPzeta3)+6720*HYPzeta3^2
        + 
        6720*HYPEulerGamma^3*HYPzeta3-1080*HYPzeta2^3+1512*HYPEulerGamma^2*HYPzeta2^2
        -
        2520*HYPEulerGamma^4*HYPzeta2+168*HYPEulerGamma^6
      )/120960*(`Sym'*`EpsSymbol')^7
      +
      HYPinvGammaExpand(`EpsSymbol'^8)*`EpsSymbol'^8
    ;
    if (count(`EpsSymbol',1) > `MaxEpsPower') discard;
  endrepeat;
#endprocedure
#procedure HypToGscheme(NumberOfLoops,GammaFuncID,InvGammaFuncID,EpsSymbol,MaxEpsPower)
  .sort
  #if (`EpsSymbol' != HYPep)
    multiply replace_(`EpsSymbol',HYPep);
  #endif
  multiply HYPep^(-`NumberOfLoops');
  multiply HYPinvGfunc(1,1,0,0)^`NumberOfLoops';
  #call HypSubstituteGfunction(HYPGfunc,HYPinvGfunc,`GammaFuncID',`InvGammaFuncID',HYPep,HYPn1,...,HYPn4)
  #call HypNormalizeGammaFunctions(HYPep,`GammaFuncID',`InvGammaFuncID',HYPnumep,HYPdenep,HYPn1,HYPn2)
  #call HypEpExpand
  #call HypSimplifyNumerics
  .sort
  PolyRatFun;
  id HYPratep(HYPn1?,HYPn2?) = HYPn1/HYPn2;
  #call HypExpandGamma(`GammaFuncID',`InvGammaFuncID',HYPep,HYPn1,`MaxEpsPower')
  #if (`EpsSymbol' != HYPep)
    multiply replace_(HYPep,`EpsSymbol');
  #endif
  #if 0
  #endif
#endprocedure
#procedure HypFibrationBasis(ExprID,LinfFuncID,LfuncID,RatFuncID,?VarList)
  #do Var={`?VarList'}
    #message Considering variable `Var'
    InExpression `ExprID';
      multiply replace_(`RatFuncID',HYPrat);
    EndInExpression;
    #call HypLimitsToHlogs(`ExprID',`LinfFuncID',`Var')
    InExpression `ExprID';
      id HYPLx(?a) = `LfuncID'(?a, `Var');
      Argument `LfuncID',`LinfFuncID';
        multiply replace_(HYPrat, `RatFuncID');
      EndArgument;
    EndInExpression;
  #enddo
#endprocedure
#procedure HypFinalizeResult(EpsSymbol,MaxEpsPower)
  .sort Finalize;
  symbols z4,z6,z8,z8,z10;
  .sort
  PolyRatFun;
  .sort
  id HYPratep(HYPn1?,HYPn2?) = HYPn1/HYPn2;
  #if (`EpsSymbol' != HYPep)
    multiply replace_(HYPep,`EpsSymbol');
  #endif
  if (count(`EpsSymbol',1) > `MaxEpsPower') discard;
  multiply replace_(
    #do imzv=3,`MAXMZVWORDLENGTH',2
      HYPzeta`imzv',z`imzv',
    #enddo
    HYPzeta2,z2
  );
  if (count(HYPLlimZero,1,HYPinf,1) > 0);
    print "Result is unreliable due to present divergences...";
    print "Affected term: %t";
  endif;
  id pi_^2 = 6*z2;
  id z2^5  = 93555/7776*z10;
  id z2^4  = 9450/1296*z8;
  id z2^3  = 945/216*z6;
  id z2^2  = 90/36*z4;
#endprocedure
#procedure HypPrintStatistics()
  #message ################################################;
  #if (`HYPcheckForZeroCoefficients')
    #message # We zeroed out `HYPzeroedOutTotal' hyperlogs;
  #endif
  #message # We used `HYPtempCount' temporary expressions;
  #message ################################################;
#endprocedure
