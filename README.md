# pltGUI
gnuplotのpltファイルをGUIから作成できるツール


## 環境
gnuplot 5.4 patchlevel 6
OpenSiv3D 0.6.6

## 使い方

1. plt Settingのwhole,graphに入力していく。
（複数のグラフを重ねて描写したいなら＋マークで増やす）
2. plt Settingからplt Fileへの矢印を押してpltファイルを生成。
3. （plt Fileのタブをクリックして中身を確認、保存）
4. plt FileからImageへの矢印を押して画像を生成。
5. 画像を確認、1.に戻って修正。
6. 気に入ったら保存。

## 画面構成

### スクショ
- plt Setting
    - whole
    ![see readme_images/whole.png](readme_images/whole.png)
    - graph1
    ![see readme_images/graph1.png](readme_images/graph1.png)
    - graph2
    ![see readme_images/graph2.png](readme_images/graph2.png)
    - graph3
    ![see readme_images/graph3.png](readme_images/graph3.png)
- plt File
![see readme_images/pltfile.png](readme_images/pltfile.png)
- plt File
![see readme_images/pltimage.png](readme_images/pltimage.png)
- app options
![see readme_images/appoption.png](readme_images/appoption.png)
![see readme_images/appoption_theme.png](readme_images/appoption_theme.png)

### 説明

最上部に

- plt Setting
- 右矢印のアイコンボタン１
- plt File
- 右矢印のアイコンボタン２
- Image
- 歯車のアイコンボタン

のUIがある。
**矢印のアイコンボタンはそれぞれ、plt Settingの入力からplt Fileの生成、plt Fileを実行して画像の生成をするボタン**。これを押さないと生成されない。押すと生成中のアニメーションが再生されるが、実行時間とは無関係。
その他の項目（歯車含む）は切り替え式のタブで、中身は以下の通り。

- plt Setting
詳細は後述
- plt File
作成したpltファイル
右上のSave asで保存できる。
- Image
作成したpltファイルから生成したグラフの画像
右上のSave asで保存できる。
- app options(歯車アイコン)
テーマなどアプリの設定
キーワードの省略（例：`linecolor`→`lc`）や、アプリのテーマカラーなどを指定できる

plt Settingのタブはその中にもタブを持つ。
- whole
軸などの全体設定
- graph1
それぞれのグラフの設定
- ＋
グラフの数を増やす。複数のグラフは重ねて描画される。削除は右のゴミ箱ボタン

whole,graph の中身は下記入力項目を参照。
グラフの数が画面端に差し掛かる場合、マウスホイールで左右にスクロールできる。

**注意**：データファイルを指定して描画する場合、生成されるplt Fileでは絶対パスで書かれるため、ユーザー名の流出などに注意。

### 入力項目
チェックなしは今後追加したいもの

- plt setting
    - whole
        - [x] title
        - [x] xrange, yrange
        - [x] logscale x, y
        - [x] sample
        - [x] xlabel, ylabel
    - graph
        - [x] 削除ボタン
        - [x] plot
        - [x] plot "datafile" 
			- [x] using x:y
        - [ ] splot
        - [x] title
        - [x] with lines
            - [x] linecolor
            - [x] linetype
            - [x] linewidth
        - [x] with points
            - [x] linecolor
            - [x] pointtype
            - [x] pointsize
        
        ※with linesとwith pointsは同時に指定できる（with linespoints）
        また、linecolorはwith pointsとwith linesで共通のため、両方に存在するが中身は同じ。
- plt file
    - [x] reload
    - [x] save as
- image
    - [x] reload
    - [x] save as
- app options
    - [x] ライセンスの表示
    - [x] pltファイルでの色の指定がrgbかhsvか
    - [x] キーワードの省略
    - [x] アプリのテーマカラーの指定（Base,Main,Accent）
- その他UI
    - [x] (plt setting -> plt file)のボタン
    - [x] (plt file -> image)のボタン
    - [x] optionsのタブをアイコンに
    - [x] チェック範囲拡大でつけやすく
    - [x] スクロールの範囲
    - [x] タブ部分と重なったときのクリック排除
    - [ ] データファイルのドラッグ＆ドロップ
    - [ ] 拡張子の設定
    - [ ] plt Fileの編集、スクロール
    - [ ] テーマカラーなどの保存
    - [ ] ボタン一つでplt生成と画像生成を行うオプション

