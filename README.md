# pltGUI
gnuplotのpltファイルをGUIから作成できるツール


## 環境
gnuplot 5.4 patchlevel 6
OpenSiv3D 0.6.6


## 画面構成
- plt Setting
    - whole (軸などの全体設定)
    - graph1(それぞれのグラフの設定)
    - ＋ (グラフの数を増やす)
- plt File
作成したpltファイル
- Image
作成したpltファイルから生成したグラフの画像
- options(歯車アイコン)
テーマなどアプリの設定

options以外の3項目間の矢印ボタンでそれぞれpltファイルと画像を生成


### 入力項目
チェックなしは今後追加したいもの

- plt setting
    - whole
        - [x] title
        - [x] xrange, yrange
        - [x] logscale x, y
        - [x] sample
    - graph
        - [x] 削除ボタン
        - [x] plot
        - [x] plot "datafile" 
			- [x] using x:y
        - [ ] splot
        - [x] title
        - [x] withlines
            - [x] linecolor
            - [x] linetype
            - [x] linewidth
        - [x] withpoints
            - [x] linecolor
            - [x] pointtype
            - [x] pointsize
- plt file
    - [x] reload
    - [x] save as
- image
    - [x] reload
    - [x] save as
- options
    - [x] pltファイルでの色の指定がrgbかhsvか
    - [x] キーワードの省略
- その他UI
    - [x] (plt setting -> plt file)のボタン
    - [x] (plt file -> image)のボタン
    - [x] optionsのタブをアイコンに
    - [x] チェック範囲拡大でつけやすく
    - [x] スクロールの範囲
    - [x] タブ部分と重なったときのクリック排除
    - [ ] データファイルのドラッグ＆ドロップ
    - [ ] 拡張子の設定

