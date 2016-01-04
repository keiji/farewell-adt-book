= Gradle Cookbook

//lead{
この章では、Antなどのビルドシステムで実現していたことをGradleでどのように実現するか、ユースケース毎に解説します。
//}

== ビルドによって含めるソースコードやリソースを入れ替えたい
「Product Flavors」を使えば、生成するAPKファイルに含めるソースコード（クラスファイル）やリソースを、ビルドのタスクに応じて入れ替えることができます。

ここでは無料の試用版（trial）と、有料版（commercial）を分ける場合を考えます。

=== Product Flavorを設定する
まず@<tt>{app/src}の下に2つのディレクトリ「trial」と「commercial」を作成します。
これらが各flavorの起点となります。

//cmd[commercialとtrialを追加した構成]{
.
|-- app.iml
|-- build
|-- build.gradle
|-- libs
|-- proguard-rules.pro
`-- src
    |-- androidTest
    |-- commercial
    |-- main
    |   |-- AndroidManifest.xml
    |   |-- java.io.keiji.farewelladt
    |   |   `-- MainActivity.java
    |   `-- res
    |-- test
    `-- trial
//}

次に、@<tt>{app/build.gradle}を開いて、@<tt>{productFlavors}の情報を記述します（@<list>{add_product_flavors}）。

//list[add_product_flavors][productFlavorsを追加]{
apply plugin: 'com.android.application'

android {

    productFlavors {
        trial
        commercial
    }
}
//}

=== クラスを入れ替える
それぞれのflavorに、切り替えたいクラスのソースコード（例. @<tt>{FooBar.java}）を配置します。

//cmd[FooBar.javaをそれぞれのflavorに配置する]{
.
|-- app.iml
|-- build
|-- build.gradle
|-- libs
|-- proguard-rules.pro
`-- src
    |-- androidTest
    |-- commercial
    |   `-- java.io.keiji.farewelladt
    |       `-- FooBar.java
    |-- main
    |   |-- AndroidManifest.xml
    |   |-- java.io.keiji.farewelladt
    |   |   `-- MainActivity.java
    |   `-- res
    |-- test
    `-- trial
        `-- java.io.keiji.farewelladt
            `-- FooBar.java
//}

この状態でビルド（@<tt>{assemble}）すると、それぞれのflavorに配置した@<tt>{FooBar.java}が組み込まれたAPKが作成されます。

====[column] クラスの重複（mainとflavor）

各flavorの下に置いたクラスを、重複して「main」に置かないように注意してください。
「main」とクラスが重複した場合、エラーが起きてビルドが完了しません。

また、flavorの下に置いたクラスで「main」から参照するメソッド、フィールドは、すべて共通にしておく必要があります。

====[/column]

「Gradle」タブで表示されるタスク一覧から@<tt>{assemble}を実行すると、すべてのflavorのビルドを一度に実行できます。
@<tt>{assembleTrial}や@<tt>{assembleCommercial}のように、flavorを個別にビルドすることもできます。

//image[as_gradle_tasks][タスク一覧][scale=0.5]{
//}

エミュレーターや実機で実行するflavorを選びたい場合、Android Studioの「Build Variants」タブ（@<img>{as_build_variants}）で切り替えることができます。

「@<tt>{Build Variants}」とは、「Product Flavors」と、デバッグ版・リリース版を切り替える「Build Types」の2つの要素を組み合わせたものを言います。

//image[as_build_variants][Build Variants - Product FlavorとBuild Typeの組み合わせ][scale=0.6]{
//}

ビルドは、コマンドラインからも実行できます。
エクスプローラー（OS Xの場合はFinder）からプロジェクトのトップにディレクトリを移動して、@<tt>{./gradlew [タスク名]}を実行します。
利用できるタスク一覧を表示するには、@<tt>{./gradlew tasks}を実行します。

//cmd[コマンドライン版のGradleはgradle wrapperを使う]{
$ ./gradlew tasks
Starting a new Gradle Daemon for this build (subsequent builds will be faster).
Parallel execution is an incubating feature.
:tasks

------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------

Android tasks
-------------
androidDependencies - Displays the Android dependencies of the project.
signingReport - Displays the signing info for each variant.
sourceSets - Prints out all the source sets defined in this project.

Build tasks
-----------
assemble - Assembles all variants of all applications and secondary packages.
assembleAndroidTest - Assembles all the Test applications.
assembleCommercial - Assembles all Commercial builds.
assembleDebug - Assembles all Debug builds.
assembleRelease - Assembles all Release builds.
assembleTrial - Assembles all Trial builds.
build - Assembles and tests this project.
buildDependents - Assembles and tests this project and all projects that depend on it.
buildNeeded - Assembles and tests this project and all projects it depends on.
clean - Deletes the build directory.

[省略]

BUILD SUCCESSFUL

Total time: 8.705 secs
//}

=== リソースを入れ替える
それぞれのflavorのディレクトリに、切り替えたいリソース（例. @<tt>{ic_launcher.png}）を配置します。

次の例では、commercialに画像リソース@<tt>{ic_launcher.png}を配置しています。

//cmd[commercialのflavorに違うアイコンを配置する]{
.
|-- app.iml
|-- build.gradle
|-- libs
|-- proguard-rules.pro
`-- src
    |-- androidTest
    |-- commercial
    |   `-- res
    |       `-- mipmap-xxxhdpi
    |           `-- ic_launcher.png
    |-- main
    |   |-- AndroidManifest.xml
    |   |-- java
    |   `-- res
    |       |-- drawable
    |       |-- layout
    |       |-- mipmap-xxxhdpi
    |       |   `-- ic_launcher.png
    |       |-- values
    |       `-- values-w820dp
    |-- test
    `-- trial
//}

この状態でビルドすると、flavor「commercial」には設定したアイコンが組み込まれたAPKが作成されます。

====[column] リソースの重複（mainとflavor）

リソースの場合は「main」のリソースと重複しても、ビルドエラーにはなりません。
flavorにリソースがあればflavorのものが優先され、なかった場合は「main」のリソースがAPKに組み込まれます。

====[/column]

== ビルドによってアプリケーションの情報を変えたい
アプリに設定するさまざまな情報（applicationId, minSdkVersion, versionCode, versionName）を、ビルドに応じて変更できます。

=== applicationIdSuffixとversionNameSuffix
@<list>{ApplicationIdSuffix}は、@<tt>{applicationIdSuffix}と@<tt>{versionNameSuffix}で情報を変更している例です。

@<tt>{applicationIdSuffix}は@<tt>{applicationId}の末尾に、@<tt>{versionNameSuffix}は@<tt>{versionName}の末尾に、それぞれ接尾辞（Suffix）を付加します。

基本となる@<tt>{applicationId}や@<tt>{versionName}は、@<tt>{defaultConfig}で設定した値です。

//list[ApplicationIdSuffix][debugビルドでapplicationIdの末尾に.debugを付加する]{
android {
    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }

    buildTypes {
        debug {
            applicationIdSuffix '.debug'
            versionNameSuffix ' debug'
        }
        release {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android.txt'),
                    'proguard-rules.pro'
        }
    }
}
//}

@<tt>{debug}については、@<tt>{applicationId}が@<tt>{io.keiji.farewelladt.debug}、@<tt>{versionName}が@<tt>{1.0 debug}に設定されます。

=== ApplicationIdの変更
ビルドするアプリの@<tt>{applicationId}を変更できます。

@<list>{application_id}の例では、ビルドするアプリのapplicationIdをまったく違うものに変更しています。

//list[application_id][ApplicationIdの変更]{
android {
    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }

    productFlavors {
        trial {
        }
        commercial {
            applicationId 'jp.foo.helloandroidstudio.commercial'
        }
    }
}
//}

=== versionCodeおよびversionNameの変更
ビルドするアプリの@<tt>{versionCode}および@<tt>{versionName}を変更できます。

@<list>{versioncode_and_version_name}では、ビルドするアプリの@<tt>{versionCode}と@<tt>{versionName}を変更しています。

//list[versioncode_and_version_name][trialとcommercialで異なるversionCodeとversionNameを設定している]{
android {
    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }

    productFlavors {
        trial {
            versionCode 30
        }
        commercial {
            versionName 'Commercial Version'
            versionCode 10001
        }
    }
}
//}

=== AndroidManifest.xmlへの反映
applicationIdSuffixを付加したときやapplicationIdそのものを変えた場合、デバッグ版とリリース版など複数のアプリを一台の端末にインストールできますが、@<tt>{AndroidManifest.xml}に記述する項目については変更されません。

すると、applicationIdが違っていても、ContentProviderのauthoritiesが重複していると、アプリがインストールができないという問題が発生します。

//image[install_failed_conflict_provider][INSTALL_FAILED_CONFLICTING_PROVIDER][scale=0.5]{
//}

AndroidManifest.xmlに、@<tt>{build.gradle}で変更したapplicationIdを反映するには、@<tt>{Manifest Merger}を使います（@<list>{androidmanifest}）。

//list[androidmanifest][providerのauthoritiesをapplicationIdで置き換える]{
<?xml version="1.0" encoding="utf-8"?>
<manifest
    package="io.keiji.farewelladt"
    xmlns:android="http://schemas.android.com/apk/res/android">

    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme">
        <activity android:name=".MainActivity">
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>

                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
        </activity>

        <provider
            android:name=".provider.SomeContentProvider"
            android:authorities="${applicationId}"/>
    </application>

</manifest>
//}

@<tt>{{applicationId}}は、ビルド時にアプリのBiuld Variantsに設定されているapplicationIdで置き換えられます。

そのほか、build.gradleとAndroidManifest.xmlとの連携については、次のURLを参照してください。

 * Android Tools Project Site - Manifest Merger
 ** http://tools.android.com/tech-docs/new-build-system/user-guide/manifest-merger

== 外部コマンドの実行結果をビルドに反映したい
ビルドの過程で外部コマンドを実行できます。

@<list>{gitsha}は、Gitで管理しているプロジェクトの（Gitの）ハッシュ値を取得する例です。
@<code>{gitSha}メソッドの中で、@<tt>{git}コマンドを実行しています。

@<tt>{buildTypes}の@<tt>{debug}内でメソッドを実行することで、@<tt>{versionName}の末尾にハッシュ値を追加しています（@<tt>{versionNameSuffix}）。

//list[gitsha][gitShaメソッド]{
apply plugin: 'com.android.application'

def gitSha() {
    return 'git rev-parse --short HEAD'.execute().text.trim()
}

android {
    buildTypes {
        debug {
            applicationIdSuffix '.debug'
            versionNameSuffix ' ' + gitSha()
        }
    }
}
//}

== 新しいBuild Typeを追加したい
最初に定義されている@<tt>{debug}と@<tt>{release}以外にもBuid Typeを追加できます。

また、@<tt>{initWith}を使うと、すでにあるBuild Type設定を引き継いで新しいBuild Typeを追加することができます。

@<list>{add_buildtype_openbeta}は、@<tt>{debug}の設定を引き継いで、新しく@<tt>{openbeta}を作成する例です。

//list[add_buildtype_openbeta][Build Typeの追加]{
    buildTypes {
        debug {
            applicationIdSuffix '.debug'
            versionNameSuffix ' ' + gitSha()
        }
        openbeta.initWith(buildTypes.debug)
        openbeta {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android.txt'),
                    'proguard-rules.pro'
        }
    }
//}

@<tt>{openbeta}をビルドすると、@<tt>{debug}の@<tt>{versionNameSuffix}と@<tt>{applicationIdSuffix}の設定を引き継いだ上で、さらに@<tt>{minifyEnabled}を有効になったAPKが生成されます。

== ビルドによって定数の内容を変えたい
アプリのビルド時に自動で生成される@<tt>{BuildConfig}には、標準でいくつかの定数が宣言されています。

@<tt>{build.gradle}を設定すると定数を追加したり、さらにBuild Variantsごとに変更したりできます（@<list>{build_config_field}）。

//list[build_config_field][buildConfigFieldによる定数の宣言]{
android {

    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"

        buildConfigField "String", "API_URL", "\"https://test.keiji.io/\""
    }

    buildTypes {
        release {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android.txt'),
                    'proguard-rules.pro'
            buildConfigField "String", "API_URL","\"https://blog.keiji.io/\""
        }
    }
}
//}

この状態でビルドをすると、@<list>{build_config_field_build_config}のように定数を追加した@<tt>{BuildConfig.java}を生成します。
また、リリースビルドでは@<tt>{release}の中で宣言した値で@<tt>{BuildConfig.java}を生成します。

//list[build_config_field_build_config][定数API_URLが追加されている]{
/**
 * Automatically generated file. DO NOT MODIFY
 */
package io.keiji.farewelladt;

public final class BuildConfig {
  public static final boolean DEBUG = Boolean.parseBoolean("true");
  public static final String APPLICATION_ID = "io.keiji.farewelladt.debug";
  public static final String BUILD_TYPE = "debug";
  public static final String FLAVOR = "trial";
  public static final int VERSION_CODE = 30;
  public static final String VERSION_NAME = "1.0";
  // Fields from default config.
  public static final String API_URL = "https://test.keiji.io/";
}
//}

== Lintのエラーでリリースビルドを中止させたくない

=====[column] 注意

Lintの指摘するエラーには対応すべき項目が数多くあります。この設定は慎重に行ってください。

=====[/column]

リリースビルド（assembleRelease）を実行するとLintがコードをチェックして、エラー項目があるとビルドを中止します。

@<tt>{lintOptions}を設定することで、Lintでエラーを指摘してもリリースビルドを中止せず、APKを生成できます（@<list>{lint_options}）。

//list[lint_options][Lintのエラーで中止しないようにする]{
android {

    lintOptions {
        abortOnError false
    }
}
//}

== 署名のキーストアに関する情報をバージョン管理に含めたくない
アプリの署名に使うキーストアの情報を@<tt>{build.gradle}に記述することはセキュリティ上、避けたいところです。

キーストアの情報をバージョン管理から切り離すには、まず、プロジェクトのトップに新しくファイル@<tt>{foo-bar.properties}を作成します。

次に、作成した@<tt>{foo-bar.properties}を@<tt>{.gitignore}に加えて、Gitの管理から外した上で、@<list>{another_properties}のようにプロパティを記述します。

//list[another_properties][foo-bar.properties]{
storeFile=[キーストアのパス（フルパス）]
storePassword=[キーストアのパスワード]
keyAlias=[キーの名前]
keyPassword=[キーのパスワード]
//}

記述したら、次は@<tt>{build.gradle}を書き換えます（@<list>{new_properties_buildgradle}）。

@<tt>{signingConfigs}で@<tt>{foo-bar.properties}があればPropertiesとして読み込み、署名に使うキーストアとして設定します。

//list[new_properties_buildgradle][プロパティファイルがあれば読み込む]{
android {

    signingConfigs {
        release {
            File propFile = rootProject.file("foo-bar.properties")
            if (propFile.exists()) {
                Properties props = new Properties()
                props.load(new FileInputStream(propFile))

                storeFile file(props.storeFile)
                storePassword props.storePassword
                keyAlias props.keyAlias
                keyPassword props.keyPassword
            }
        }
    }

    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'),
                    'proguard-rules.pro'

            signingConfig signingConfigs.release
        }
    }
}
//}
