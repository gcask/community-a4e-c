#include <vector>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include "FlightModel.h"
#include "Globals.h"

//Tables
#define d_CLalpha {-0.62,-0.5929322197774497,-0.5658644395548995,-0.5387966593323492,-0.511728879109799,-0.48466109888724873,-0.45759331866469843,-0.43052553844214825,-0.40345775821959795,-0.3763899779970477,-0.3493221977744975,-0.3222544175519472,-0.295186637329397,-0.26811885710684674,-0.2410510768842965,-0.21398329666174626,-0.18290710325079987,-0.14639046545254625,-0.10987382765429264,-0.07335718985603912,-0.0368405520577855,-0.0003239142595318767,0.042643546110487186,0.0857488175633278,0.1288540890161682,0.17195936046900862,0.21443151629351237,0.256860889066799,0.2992902618400856,0.3417196346133724,0.3839807976005338,0.42606148090253004,0.4681421642045261,0.5102228475065221,0.5523035308085184,0.5930871583321505,0.6335069949277878,0.6739268315234249,0.714346668119062,0.7547665047146993,0.7953618508583834,0.8360441418068678,0.8705066082263391,0.8994578452099521,0.9212646678844486,0.9336366173395014,0.9348031740232255,0.9235012999676309,0.8970901677354586,0.8771220328342895,0.863298529314007,0.8528257868596284,0.8478470339374391,0.8428682810152499,0.8379124795129725,0.8355784298812838,0.833244380249595,0.8300110854300122,0.8136781166212667,0.7784223543862703,0.7174176301114356,0.6699784618581941,0.6279601605284113,0.585941859198629,0.5706247231504185,0.581309044811902,0.5919933664733855,0.6026776881348691,0.6133620097963527,0.6240463314578362,0.6347306531193196,0.6454149747808032,0.65546308508979,0.659757058331076,0.664051031572362,0.668345004813648,0.672638978054934,0.67693295129622,0.681226924537506,0.685520897778792,0.689814871020078,0.694108844261364,0.69840281750265,0.6884703221691076,0.6778291501329559,0.6671879780968041,0.6565468060606523,0.6459056340245004,0.6352644619883487,0.6246232899521968,0.6139821179160452,0.6033409458798933,0.5926997738437414,0.5820586018075897,0.5714174297714378,0.5607762577352862,0.5501350856991343,0.5394939136629825,0.5288527416268307,0.5182115695906789,0.5075703975545273,0.4969292255183754,0.4862880534822235,0.4756468814460718,0.4650057094099199,0.45436453737376825,0.44372336533761636,0.4330821933014645,0.42244102126531285,0.41179984922916096,0.4011586771930093,0.3905175051568574,0.37987633312070573,0.36923516108455384,0.358593989048402,0.34795281701225034,0.33731164497609845,0.3266704729399468,0.3160293009037949,0.305388128867643,0.2947469568314913,0.2841057847953395,0.2734646127591878,0.26282344072303593,0.25218226868688404,0.24154109665073237,0.23089992461458048,0.2202587525784288,0.20961758054227697,0.19897640850612508,0.1883352364699734,0.17769406443382152,0.16705289239766985,0.15641172036151796,0.14577054832536607,0.1351293762892144,0.1244882042530625,0.11384703221691084,0.10320586018075906,0.09256468814460717,0.0819235161084555,0.0712823440723036,0.060641172036151936,0.05}
#define d_CDalpha {1.55,1.5195341820590185,1.489068364118037,1.4586025461770549,1.4281367282360733,1.3976709102950917,1.3672050923541101,1.3367392744131283,1.3062734564721465,1.275807638531165,1.2453418205901832,1.2148760026492016,1.18441018470822,1.1539443667672382,1.1234785488262564,1.0930127308852748,1.0625469129442933,1.0320810950033117,1.0016152770623297,0.9711494591213482,0.9406836411803665,0.9102178232393848,0.8797520052984031,0.8492861873574216,0.8188203694164398,0.7883545514754582,0.7578887335344764,0.7274229155934948,0.6969570976525132,0.6664912797115314,0.6360254617705499,0.6055596438295681,0.5750938258885865,0.5446280079476049,0.5141621900066231,0.48369637206564153,0.45323055412465973,0.42276473618367816,0.3914953271028038,0.3578504672897196,0.32420560747663557,0.29056074766355156,0.253644859813084,0.21158878504672893,0.1696028037383178,0.13385514018691597,0.09810747663551417,0.07567757009345791,0.06095794392523363,0.04892523364485982,0.04471962616822431,0.0405140186915888,0.03630841121495326,0.03210280373831775,0.0291588785046729,0.029831775700934565,0.035771028037383155,0.04207943925233647,0.048387850467289724,0.06095794392523363,0.07567757009345791,0.09810747663551386,0.13385514018691597,0.16960280373831776,0.21158878504672893,0.253644859813084,0.2905607476635512,0.32420560747663557,0.35785046728971964,0.3914953271028037,0.4227647361836779,0.4532305541246595,0.4836963720656414,0.514162190006623,0.5446280079476047,0.5750938258885863,0.6055596438295678,0.6360254617705497,0.6664912797115314,0.696957097652513,0.7274229155934946,0.7578887335344763,0.788354551475458,0.8188203694164395,0.8492861873574217,0.8797520052984033,0.9102178232393849,0.9406836411803665,0.9711494591213481,1.0016152770623297,1.0320810950033112,1.062546912944293,1.0930127308852746,1.1234785488262564,1.1539443667672384,1.18441018470822,1.2148760026492016,1.2453418205901832,1.2758076385311647,1.3062734564721465,1.3367392744131281,1.36720509235411,1.3976709102950915,1.428136728236073,1.458602546177055,1.4890683641180367,1.5195341820590182,1.55}
#define d_CDmach {0.0,0.0,0.0,0.0,0.0,0.000940909090909,0.0197590909091,0.0216283084005,0.0199712313003,0.0183141542002,0.0166570771001,0.015 }
#define d_CDbeta {1.23,0.978284308116,0.731177185308,0.501113771365,0.300529206075,0.141858629225,0.0375371806043,0.0,0.0375371806043,0.141858629225,0.300529206075,0.501113771365,0.731177185308,0.978284308116,1.23 }
#define d_CLflap {0.0,0.010619970851709465,0.021239941703418887,0.03185991255512836,0.044161996549066614,0.05651283744567813,0.06886367834228971,0.08121451923890129,0.09356536013551292,0.10591620103212454,0.11826704192873612,0.1306178828253477,0.1429687237219593,0.1553195646185709,0.1676704055151825,0.18002124641179412,0.19237208730840574,0.20472292820501728,0.21707376910162884,0.22942460999824044,0.24177545089485203,0.2541262917914637,0.26401677179574484,0.2733029526334888,0.28258913347123277,0.28510550964879355,0.28565248810702837,0.2861994665652631,0.28682154083075606,0.28746911933267627,0.2881166978345964,0.28983850718477105,0.29198158353426124,0.29412465988375136,0.29575958414278924,0.2971663584836649,0.2985731328245406,0.2997216840395095,0.3007383766369942,0.3017550692344788,0.30198510194883293,0.3017606200640449,0.30153613817925695,0.3012778239502455,0.30099747935755367,0.3007171347648619,0.29988477205706465,0.2986484936552876,0.2974122152535107,0.29628787224219455,0.29525537365382076,0.294222875065447,0.2940360516068376,0.29462633502422797,0.2952166184416184,0.29602534454664015,0.29705875455894004,0.29809216457124005,0.29891359315101623,0.29949092189940185,0.30006825064778736,0.30069244537415446,0.3013771123301752,0.3020617792861958,0.3026906962707837,0.30323887191743437,0.3037870475640849,0.3053147621502583,0.30843876241565393,0.31156276268104943,0.314856253652866,0.31846160752449715,0.3220669613961281,0.3252532306540079,0.327564888544059,0.32987654643411013,0.33242073875671685,0.33551858449017014,0.33861643022362364,0.34016568703232547,0.3374767004680228,0.3347877139037202,0.33038148086867225,0.32052046492655145,0.3106594489844306,0.3012115905959168,0.293306187074203,0.2854007835524891,0.2756807385287999,0.25786460065014377,0.24004846277148775,0.21977397995177272,0.18609034290809823,0.15240670586442379,0.1216648881031212,0.11118893650927136,0.10071298491542147,0.09126987362345684,0.09126987366301376,0.09126987370257067,0.09126987374212764,0.0912698737816845,0.09126987382124135,0.09126987386079821,0.09126987390035514,0.09126987393991204,0.09126987397899529,0.09126987398492276,0.09126987399085014,0.0912698739967776,0.09126987392577375,0.09126987385367082,0.09126987378156792,0.0912698737094651,0.09126987363736229,0.09126987356525945,0.09126987349315663,0.09126987342105375,0.09126987334895083,0.09126987327684806,0.09126987320474535,0.09126987313264256,0.09102786658719303,0.090750730069806,0.09047359355241903,0.08954068459310105,0.0884875506860957,0.08743441677909038,0.08638128287208496,0.08532814896507963,0.08427501505807429,0.08322188115106893,0.08216874724406366,0.0811156133370583,0.08006247943005301,0.07900934552304767,0.0779562116160424,0.07690307770903705,0.07584994380203164,0.07479680989502628,0.07374367598802094,0.07269054208101561,0.07163740817401032,0.07058427426700496,0.0695311403599996,0.0684780064529942,0.0674248725459888,0.06637173863898349,0.06531860473197812,0.0642654708249728,0.06321233691796746,0.06215920301096218,0.061106069103956856,0.060052935196951485,0.05899980128994611,0.05794666738294077,0.056893533475935426,0.055840399568930055,0.05478726566192474,0.05373413175491937,0.05268099784791402,0.05162786394090871,0.05057473003390342,0.049521596126898104,0.04846846221989277,0.04741532831288737,0.04636219440588196,0.045309060498876626,0.04425592659187134,0.04320279268486607,0.04214965877786076,0.04109652487085538,0.04004339096385004,0.03899025705684464,0.0379371231498393,0.03688398924283395,0.03583085533582862,0.03477772142882322,0.03372458752181787,0.03267145361481252,0.03161831970780718,0.030565185800801833,0.029512051893796507,0.028458917986791163,0.027405784079785847,0.026352650172780517,0.025299516265775188,0.02424638235876989,0.02319324845176457,0.022140114544759202,0.021086980637753834,0.02003384673074847,0.018980712823743126,0.017927578916737782,0.016874445009732425,0.01582131110272708,0.01476817719572174,0.013715043288716408,0.012661909381711078,0.011608775474705763,0.010555641567700426,0.009502507660695056,0.008449373753689662,0.007396239846684319,0.006343105939678975,0.0052899720326736255,0.004236838125668282,0.0031837042186629305,0.00213057031165758,0.0010774364046522364,2.4302497646951776e-05,2.710505431213761e-20,-0.0010531339070052846,-0.0021062678140106677,-0.0031594017210160494,-0.004212535628021433}
#define d_CLslat {0.0,-5.421010862427522e-20,-0.005241367037379541,-0.010482734074759359,-0.01572410111213912,-0.020965468149518773,-0.02620683518689848,-0.03144820222427819,-0.03136838365324465,-0.016043570257264658,-0.0007187568612846969,0.0027643509061028027,-0.0025606159718781774,-0.006702936232427181,-0.007778190966405585,-0.008611569124549414,-0.010116588440471752,-0.01160205420358229,-0.012015691432656796,-0.014312977300064533,-0.016859703519138125,-0.011948849948414275,0.020861166735100634,0.08542871229808735,0.18621064665469,0.2672633244766701,0.311997547099532,0.3391558772793602,0.35075235209595795,0.3324828225887487,0.3083812317884639,0.3096630805291034,0.3103356359373798,0.3103356359373798,0.3103356359373798,0.3103356359373798,0.3103356359612922,0.3103356360406885,0.31033563612008463,0.3103356361994809,0.31033563627887706,0.3076826810059532,0.29760130387366746,0.2875199267413816,0.2774385496090959,0.2673571724768099,0.25727579534452427,0.24719441821223853,0.23711304107995268,0.22703166394766705,0.2169502868153811,0.20686890968309535,0.19678753255080958,0.18670615541852392,0.176624778286238,0.16654340115395228,0.15646202402166642,0.1463806468893806,0.1362992697570949,0.1262178926248092,0.1161365154925234,0.10605513836023762,0.09597376122795169,0.08589238409566607,0.07581100696338028,0.06572962983109443,0.05564825269880869,0.045566875566522946,0.035485498434237095,0.025404121301951358,0.015322744169665616,0.005241367037379763}
#define d_CDflap {0.05,0.049560898259073005,0.04912179651814601,0.04868269477721903,0.04824359303737637,0.04780449129922046,0.04736538956106454,0.04692628782290863,0.04648718608565778,0.04604808434865831,0.045608982611658834,0.045169880874458244,0.04473077913630233,0.04429167739814644,0.043852575659990525,0.04341347392026835,0.04297437217934135,0.04253527043841435,0.042096168697487384,0.04165706695921099,0.04121796522105507,0.04077886348289916,0.04033976174514548,0.03990066000814602,0.03946155827114654,0.03902245653414707,0.03858335479424464,0.038144253053317645,0.03770515131239067,0.03726604957182512,0.03682694783366921,0.03638784609551329,0.035948744357357384,0.03550964261775571,0.03507054087682871,0.03463143913590171,0.034192337394974716,0.03375323566016384,0.03331413392593548,0.032875032191707086,0.032435930455439986,0.031996828714512995,0.031557726973586,0.03111862523265902,0.03067952349365973,0.03024042175550382,0.029801320017347907,0.02936221827895103,0.028923116538024055,0.028484014797097057,0.02804491305617006,0.027605811318446742,0.027166709584218348,0.026727607849989957,0.026288506115761584,0.025849404375708318,0.02541030263478132,0.024971200893854323,0.024532099152927346,0.02409299741200035,0.02365389567107335,0.023214793930146353,0.022775692191026587,0.022336590452870675,0.021897488714714784,0.021458386976438388,0.021019285235511397,0.020580183494584407,0.020141081753657437,0.019701980015642877,0.019262878281414483,0.018823776547186092,0.018384674812957698,0.017945573073195675,0.017506471332268698,0.0170673695913417,0.016628267851017113,0.0161891661128612,0.015750064374705306,0.015310962636549396,0.014871860896706736,0.01443275915577974,0.013993657414852742,0.013554555673925746,0.013115453936926291,0.012676352199926815,0.01223725046292734,0.011798148725475346,0.011359046987319444,0.01091994524916354,0.010480843511007629,0.01004174177155051,0.009602640031731948,0.009163538291913395,0.008724436552094842,0.00828533481227628,0.007846233072457716,0.007407131332639153,0.0069680295950405036,0.0065289278591494715,0.00608982612325843,0.0056507243873673875,0.005211622647984647,0.004772520908443202,0.004333419168901748,0.0038943174292639187,0.0034552156894453586,0.0030161139496268086,0.0025770122098082485,0.0021379104700920957,0.0016988087304120971,0.0012597069907320881,0.0008461488126692636,0.0006028810300364106,0.00035961324740355755,0.00011634546477071,-0.00022909655356699558,-0.0006681982933855486,-0.0011073000332041116,-0.0015464017730226747,-0.001985503512588216,-0.0024246052521296703,-0.0028637069916711147,-0.0033028087312969067,-0.0037419104711154698,-0.004181012210934023,-0.004620113950752586,-0.005059215690571139,-0.005498317430389702,-0.005937419170208265,-0.006376520909661198,-0.006815622645275131,-0.007254724380889054,-0.007693826116502988,-0.00813292785412783,-0.008572029593946384,-0.009011131333764947,-0.0094502330735835,-0.009889334813402062,-0.010328436553220625,-0.010767538293039178,-0.011206640033146899,-0.011645741774073884,-0.012084843515000882,-0.012523945255927878,-0.012963046992486208,-0.0134021487267146,-0.013841250460942981,-0.014280352195462616,-0.014719453936389612,-0.0151585556773166,-0.015597657418243597,-0.016036759157965763,-0.016475860896121675,-0.01691496263427759,-0.017354064372433493,-0.017793166112878563,-0.018232267853805547,-0.018671369594732545,-0.01911047133420332,-0.019549573068431705,-0.019988674802660096,-0.020427776536888476,-0.02086687827519428,-0.021305980016121273,-0.02174508175704826,-0.022184183497975258,-0.02262328523613116,-0.023062386974287075,-0.023501488712442987,-0.023940590451683227,-0.024379692192610224,-0.02481879393353721,-0.02525789567446421,-0.025696997412317492,-0.026136099149316958,-0.02657520088631643,-0.027014302623517016,-0.02745340436167293,-0.02789250609982884,-0.028331607837984742,-0.02877070957770692,-0.029209811318633903,-0.029648913059560897,-0.03008801480048789,-0.030527116538764276,-0.03096621827692019,-0.031405320015076094,-0.03184442175282977,-0.03228352348982924,-0.03272262522682871,-0.033161726963828186,-0.03360082870168242,-0.03403993043983832,-0.03447903217799424,-0.0349125716013082,-0.03530902892298854,-0.035705486244668884,-0.036101943566349215,-0.03645128480220127,-0.036757436292606624,-0.037063587783011984,-0.03736973927341734,-0.03767589076382269,-0.03798204225422805,-0.0382881937446334,-0.0385301694778229,-0.038625457776087425,-0.038720746074351955,-0.03881603437261648,-0.038539714502550206,-0.03810081605114171,-0.03766191759973321,-0.03722301914832472,-0.036784120696916243,-0.03634522224550775,-0.035906323794099256,-0.03539271892123689,-0.03479761613576369,-0.034202513350290516,-0.033607410564817315,-0.03259436977102313,-0.03151863828307275,-0.03044290679512237,-0.029367175303939102,-0.028291443803595814,-0.027215712303252527,-0.02613998080290924,-0.025000181946217778,-0.02382621383217318,-0.022652245718128523,-0.021461969548674683,-0.019912916189621318,-0.01836386283056795,-0.016814809471514654,-0.015265756118485381,-0.013716702773287428,-0.012167649428089474,-0.010618596082891521,-0.009069542726018946,-0.0075204893666884695,-0.005971436007357992,-0.0044223826479913705,-0.002873329288494628,-0.0013242759289979564,0.00022477743049878571,0.0017738307899617947,0.003322884149403118,0.00487193750884444,0.006420990868285692,0.007970044215146292,0.00951909756200689,0.01106815090886749,0.012617204260499172,0.014166257619552473,0.015715310978605845,0.017264364337659217,0.018813417696712586,0.020362471055765958,0.021911524414819257,0.02344970573725515,0.024936244885685965,0.026422784034116784,0.027909323182547595,0.029056654803698827,0.029943057558733787,0.030829460313768747,0.0317158630688037,0.032602265821946426,0.03348866857500309,0.0343750713280598,0.03526443918184088,0.036159366599478646,0.03705429401711642,0.03794922143475415,0.038949162839263335,0.039986168003609705,0.041023173167956076,0.042060178332061986,0.04309718349456477,0.0441341886570676,0.04517119381957043,0.04621514579075447,0.047265465669888096,0.048315785549021675,0.0493661054281553,0.051048584790513774,0.05279126981681629,0.0545339548431188,0.05627663986876762,0.05801932489292254,0.05976200991707746,0.06150469494123238,0.06324737996688128,0.06499006499318372,0.06673275001948623,0.0684428358813826,0.06981063051061852,0.07117842513985445,0.07254621976909031,0.0739105664098937,0.07527115160876587,0.07663173680763803,0.07799232200651018,0.0793529072019144,0.08071349239679851,0.08207407759168261,0.08341835473346142,0.0847164257244821,0.08601449671550274,0.08731256770652343,0.08860819249031464,0.08990251263024977,0.09119683277018492,0.09249115290994918,0.09378547304595554,0.09507979318196191,0.09637411331796827,0.0976162476810827,0.09879054053917188,0.09996483339726112,0.10113912625535035,0.10230309068539159,0.10346488070953101,0.10462667073367038,0.10578846075780982,0.10695025078194924,0.10811204080608866,0.10927383083022808,0.11009713811545528,0.11070284936834175,0.11130856062122822,0.11191427187411468,0.11251998312700115,0.11312569437988759,0.11373140563277406,0.11433711688566052,0.11494282813854699,0.11554853939143346,0.11615425064431989,0.11675996189720636,0.11736567315009283,0.1179713844029793,0.11857709565586576,0.1191828069087522,0.11978851816163867,0.12039422941452513,0.1209999406674116,0.12160565192029807,0.12221136317318451,0.12281707442607097,0.12342278567895744,0.12402849693184391,0.12463420818473038,0.1252399194376168,0.12584563069050328,0.12645134194338975,0.12705705319627622,0.1276627644491627,0.12826847570204913,0.12887418695493558,0.12948192436207945,0.1301031694658006,0.13072441456952177,0.1313456596732429,0.13179498822410185,0.13208672659821172,0.13237846497232159,0.13267020334643145,0.1329619417205413,0.1332536800946512,0.13354541846876106,0.13355388443238644,0.13291487063127455,0.13227585683016266,0.13163684302905077,0.13099782922793887,0.13035881542682695,0.12971980162571506,0.1290807878246032,0.12844177402349127,0.12780276022237938,0.1271637464212675,0.12652473262015557,0.12588571881904367,0.1252467050179318,0.12460769121681989,0.123968677415708,0.12332966361459609,0.12269064981348418,0.12205163601237232,0.12141262221126041,0.1207736084101485,0.12013459460903661,0.1194955808079247,0.11885656700681282,0.11821755320570093,0.11757853940458902,0.11693952560347712,0.11630051180236523,0.11566149800125335,0.11502248420014144,0.11438347039902955,0.11374445659791764,0.11310544279680573,0.11246642899569385,0.11182741519458196,0.11118840139347005,0.11054938759235815,0.10991037379124624,0.10927135999013438,0.10863234618902247,0.10799333238791056,0.10735431858679867,0.10671530478568676,0.10607629098457486,0.10543727718346296,0.10479826338235111,0.1041592495812392,0.10352023578012731,0.1028812219790154,0.1022422081779035,0.1016031943767916,0.1009641805756797,0.10032516677456779,0.0996861529734559,0.09904713917234398,0.09840812537123214,0.09776911157012025,0.09713009776900833,0.09649108396789643,0.09585207016678454,0.09521305636567262,0.09457404256456073,0.09393502876344882,0.09329601496233692,0.09265700116122502,0.09201798736011317,0.09137897355900126,0.09073995975788937,0.09010094595677746,0.08946193215566556,0.08882291835455366,0.08818390455344176,0.08754489075232985,0.08690587695121796,0.08626686315010605,0.0856278493489942,0.08498883554788231,0.0843498217467704,0.0837108079456585,0.0830717941445466,0.08243278034343468,0.08179376654232279,0.0811547527412109,0.08051573894009897,0.07987672513898708,0.07923771133787524,0.07859869753676332,0.07795968373565143,0.07732066993453952,0.07668165613342762,0.07604264233231572,0.07540362853120382,0.07476461473009191,0.07412560092898002,0.07348658712786811,0.07284757332675626,0.07220855952564437,0.07156954572453246,0.07093053192342055,0.07029151812230866,0.06965250432119675,0.06901349052008485,0.06837447671897294,0.06773546291786105,0.06709644911674914,0.06645743531563729,0.0658184215145254,0.06517940771341349,0.06454039391230158,0.06390138011118969,0.06326236631007778,0.06262335250896588,0.06198433870785397,0.06134532490674208,0.06070631110563017,0.06006729730451833,0.059428283503406426,0.05878926970229452,0.05815025590118261,0.05751124210007072,0.05687222829895881,0.056233214497846906,0.05559420069673501,0.05495518689562311,0.0543161730945112,0.05367715929339936,0.053038145492287456,0.05239913169117555,0.05176011789006364,0.05112110408895175,0.05048209028783984,0.049843076486727936,0.04920406268561604,0.048565048884504136,0.04792603508339223,0.04728702128228039,0.046648007481168485,0.04600899368005658,0.045369979878944686,0.04473096607783278,0.04409195227672087,0.043452938475608965,0.04281392467449707,0.042174910873385166,0.04153589707227326,0.040896883271161366,0.040257869470049515,0.03961885566893761,0.038979841867825715,0.03834082806671381,0.0377018142656019,0.037062800464489995,0.0364237866633781,0.035784772862266195,0.03514575906115429,0.034506745260042396,0.033867731458930544,0.03322871765781864,0.032589703856706745,0.03195069005559484,0.03131167625448293,0.03067266245337104,0.03003364865225913,0.029394634851147225,0.028755621050035318,0.028116607248923425,0.027477593447811574,0.026838579646699667,0.026199565845587774,0.025560552044475868,0.02492153824336396,0.024282524442252068,0.02364351064114016,0.023004496840028255,0.022365483038916348,0.021726469237804455,0.021087455436692604,0.02044844163558071,0.019809427834468804,0.019170414033356897,0.01853140023224499,0.017892386431133098,0.01725337263002119,0.016614358828909284,0.015975345027797377,0.015336331226685485,0.014697317425573633,0.01405830362446174,0.013419289823349834,0.012780276022237927,0.01214126222112602,0.011502248420014127,0.01086323461890222,0.010224220817790314,0.009585207016678421,0.008946193215566514,0.008307179414454663,0.00766816561334277,0.007029151812230849,0.0063901380111189565,0.005751124210007064,0.005112110408895143,0.00447309660778325,0.0038340828066713573,0.0031950690055594366,0.0025560552044475437,0.0019170414033357064,0.0012780276022238135,0.0006390138011119206,0.0}
#define d_CDslat {0.0,0.0021393552221065093,0.004278710444213463,0.006418065666319972,0.008557420888426705,0.010696776110533435,0.012836131332639944,0.014975486554746675,0.017114841776853407,0.019254196998959916,0.021393552221066647,0.02353290744317338,0.02567226266527989,0.02781161788738662,0.029950973109493243,0.03209032833159997,0.0342296835537067,0.03636903877581333,0.03850839399791983,0.03999999999999998,0.03999999999999998,0.04990566037735845,0.072688679245283,0.08457547169811315,0.07183962264150942,0.048207547169811356,0.028490566037735827,0.013207547169811278,0.0050000000000000044,0.004386792452830192,0.00014150943396225345,0.0,-0.020566037735849002,-0.016792452830188626,0.004603773584905797,0.017305006685485153,0.023723072351805236,0.030141138018125152,0.036559203684445235,0.04297726935076521,0.04939533501708518,0.05581340068340525,0.062231466349725234,0.06864953201604532,0.07506759768236518,0.08148566334868527,0.08790372901500523,0.09432179468132565,0.10073986034764518,0.10715792601396534,0.11357599168028555,0.11999405734660552,0.12641212301292554,0.13283018867924526}
#define d_Cmalpha {-0.3785964243406269,-0.379201560304398,-0.3798298142306464,-0.3804826011305135,-0.3811613360151408,-0.3818674338956694,-0.3826023097832405,-0.3833673786889958,-0.38416405562407624,-0.3849937555996234,-0.3858578936267785,-0.386757884716683,-0.38769514388047793,-0.38867108612930484,-0.38968712647430503,-0.3907446799266197,-0.3918451614973904,-0.3929899861977582,-0.3941805690388646,-0.39541832503185076,-0.39670466918785807,-0.398041016518028,-0.39942878203350163,-0.4008612653459438,-0.40228289344316953,-0.4036198863949109,-0.4047984334446556,-0.4057447238358919,-0.40638494681210763,-0.40664529161679075,-0.4065358547853261,-0.406898553839063,-0.4090508148554548,-0.4143156487900599,-0.4240310300915852,-0.4395838627338156,-0.4626997767112956,-0.4993521492742275,-0.5563506071803614,-0.6379533475704406,-0.8534408479115869,-1.0689006527803038,-1.1484598021636132,-1.1975003194361444,-1.232465516537084}
#define d_Cmadot {-0.9993130309442757,-1.0018187102761391,-1.0057137412443482,-1.0109586899351304,-1.017514122434713,-1.0253406048293243,-1.0343987032051913,-1.0446489836485415,-1.0560520122456027,-1.0685683550826022,-1.0821585782457674,-1.096783247821326,-1.1124029298955054,-1.1289781905545333,-1.146469595884637,-1.1648377119720439,-1.1840431049029818,-1.2040524714147427,-1.2250244063345275,-1.2473428282151473,-1.27140452546783,-1.297606306162126,-1.3263780089078283,-1.358250017120115,-1.3937717760511161,-1.4336332032738917,-1.4799692856776496,-1.535765202640427,-1.603674886754863,-1.6830194056536476,-1.7712396054032018,-1.86566837018059,-1.9626868506773845,-2.0411573489737553,-2.0725081152125373,-2.051629767078606,-1.9928202871720833,-1.8764916624814476,-1.5999447971294505}
#define d_Cmq {-3.5094277292596345,-3.5161567596970422,-3.5231526365828003,-3.530683596512638,-3.539017876082285,-3.5484237118874686,-3.5591693405239178,-3.571522998587362,-3.585752922673529,-3.6021273493781476,-3.6209145152969455,-3.642382657025654,-3.6668000111599985,-3.69442933986108,-3.72540943096835,-3.7597561650252067,-3.7974801615505402,-3.8385954276499694,-3.8832158394602985,-3.931569116201113,-3.9838891745237586,-4.0404129917232225,-4.101398343566679,-4.167111659804254,-4.237871697448557,-4.314912123940566,-4.400245407307992,-4.496195029307437,-4.606819861954952,-4.7368814605769884,-4.89221154553636,-5.077795536462455,-5.29021819476004}
#define d_Cmde {-0.42433674491161866,-0.43360365367346587,-0.44358321449012156,-0.4541683805320369,-0.465252104969664,-0.47672734097345454,-0.4884870417138602,-0.5004241603613329,-0.5124316500863241,-0.5244024640592858,-0.5362295554506697,-0.5478058774309277,-0.5590217544751415,-0.5697379588056424,-0.5797966638913798,-0.5890397535527863,-0.5973031012480179,-0.6043628489891902,-0.6099609273584495,-0.6138388629668243,-0.6157381824253423,-0.6154079015957615,-0.6128051380678015,-0.6081172206958865,-0.6011228713333804,-0.5856775155904658,-0.5530458025153671,-0.5095060121338942,-0.46663952741682035,-0.429251669968572,-0.3991974591512412,-0.37544477220689737,-0.3566799827137331}
#define d_CLde {0.307844559886235,0.3139230198379101,0.3197154500111718,0.3252526572503578,0.33056544839980484,0.33568463030385026,0.34064100980683176,0.3454653937530864,0.35018858898695154,0.35484140235276423,0.35945464069486216,0.3640591108575824,0.36868561968526226,0.373364974022239,0.3781278747383978,0.3829572489660009,0.3877130218373679,0.39223559310386064,0.3963824229295535,0.4001718619410713,0.40371067038340475,0.4070546553345904,0.4100006703747274,0.4122649171359812,0.4135286046368574,0.4132278501058644,0.41064825679954386,0.40339364783874804,0.3873881193466855,0.36446721821468625,0.3397687181340031,0.3159940063863457,0.29458000088650843,0.27553783634235113,0.2587491400655817,0.2440955393679079}
#define d_CmM {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,-9.560815312029521e-05,-0.00019681003256176018,-0.00029801191200322515,-0.0003992137914446901,-0.000500415670886155,-0.00060161755032762,-0.0007028194297690849,-0.0008040213092105503,-0.0009052231886520148,-0.0010064250680934795,-0.0011076269475349448,-0.0012088288269764102,-0.0013100307064178747,-0.0014112325858593392,-0.0015124344653008046,-0.00161363634474227,-0.0017148382241837345,-0.0018160401036251992,-0.0019172419830666644,-0.00201844386250813,-0.0021331218479256294,-0.006662712162585601,-0.011192302477245604,-0.015721892791905605,-0.024748364761256903,-0.040268298595628874,-0.05578823243000096,-0.07130816626437303,-0.09883705281405579,-0.13391253753046684,-0.16898802224687814,-0.20406350696328945,-0.2768152998838925,-0.34744998081556966,-0.36950708685474104,-0.2693820864264927,-0.13469104321324635,0.0}



//Set everything to zero.
//Vectors already constructor to zero vector.
Skyhawk::FlightModel::FlightModel(Input& controls, Airframe& airframe, Engine& engine):
	m_controls(controls),
	m_airframe(airframe),
	m_engine(engine),
	m_density(0.0),
	m_speedOfSound(0.0),
	m_aoa(0.0),
	m_beta(0.0),
	m_scalarVSquared(0.0),
	m_scalarV(0.0),
	m_aileronLeft(0.0),
	m_aileronRight(0.0),
	m_elevator(0.0),
	m_rudder(0.0),
	m_q(0.0),
	m_p(0.0),
	m_k(0.0),
	m_kL(0.0),
	m_kR(0.0),
	m_mach(0.0),
	m_aoaDot(0.0),
	m_aoaPrevious(0.0),

	m_scalarAirspeedLW(0.0),
	m_scalarAirspeedRW(0.0),

	m_rLW(0, 0, -2.0),	//directional vbector from CG to CP of aerodynamic element (LW)
	m_rRW(0, 0, 2.0),	//directional vbector from CG to CP of aerodynamic element (RW)
	m_aoaLW(0.0),
	m_aoaRW(0.0),

	//Setup tables
	CLalpha(d_CLalpha, -0.26981317, 1.57079633),
	dCLflap(d_CLflap, -0.26981317, 1.57079633),
	dCLslat(d_CLslat, -0.26981317, 1.57079633),
	CLde(d_CLde, 0.157982, 1.000377),

	CDalpha(d_CDalpha, -1.57079633, 1.57079633),
	CDi({0.09}, 0, 1),
	CDmach(d_CDmach,0.0, 1.8),
	CDflap(d_CDflap, -1.57079633, 1.57079633),
	CDslat(d_CDslat, -1.57079633, 1.57079633),
	CDspeedBrake({0.021}, 0.0, 1.0),
	CDbeta(d_CDbeta,-1.57, 1.57),
	CDde({0.12}, c_elevatorDown, c_elevatorUp),

	CYb({-1}, 0.0, 1.0),

	Clb({ -0.01 }, 0.0, 1.0),
	Clp({ -0.3 }, 0.0, 1.0),
	Clr({ 0.15 }, 0.0, 1.0),
	Cla({ 0.220, 0.037 }, 0.0, 2.0), //110
	Cldr({ 0.01 }, 0.0, 1.0),

	Cmalpha(d_Cmalpha, 0.4004, 1.0019),
	Cmde(d_Cmde, 0.2022704, 1.00216078), //x = mach
	Cmq(d_Cmq, 0.13176098, 1.0006616),
	Cmadot(d_Cmadot, 0.1618916, 0.99790229),
	CmM(d_CmM, 0.06761245, 1.0),

	Cnb({0.12}, 0.0, 1.0),
	Cnr({-0.15}, 0.0, 1.0),
	Cndr({-0.1}, 0.0, 1.0),
	Cnda({0.0}, 0.0, 1.0)
{

}

Skyhawk::FlightModel::~FlightModel()
{

}

void Skyhawk::FlightModel::coldInit()
{

}

void Skyhawk::FlightModel::hotInit()
{

}

void Skyhawk::FlightModel::airbornInit()
{

}

void Skyhawk::FlightModel::calculateLocalPhysicsParams()
{
	double m_wingDihedral = 0.046774824; // 2.68 deg

	m_nRW = Vec3(0.0, cos(m_wingDihedral), -sin(m_wingDihedral));
	m_nLW = Vec3(0.0, cos(m_wingDihedral), sin(m_wingDihedral));

	m_airspeedLW = m_airspeedLocal + cross(m_omega, m_rLW);
	m_airspeedRW = m_airspeedLocal + cross(m_omega, m_rRW);

	m_dragVecLW = -normalize(m_airspeedLW);
	m_dragVecRW = -normalize(m_airspeedRW);

	Vec3 tmpNormalLW{ normalize(cross(m_dragVecLW, m_nLW)) };
	Vec3 tmpNormalRW{ normalize(cross(m_dragVecRW, m_nRW)) };
	m_liftVecLW = normalize(cross(tmpNormalLW, m_dragVecLW));
	m_liftVecRW = normalize(cross(tmpNormalRW, m_dragVecRW));

	m_scalarAirspeedLW = sqrt(m_airspeedLW.x * m_airspeedLW.x + m_airspeedLW.y * m_airspeedLW.y + m_airspeedLW.z * m_airspeedLW.z);
	m_scalarAirspeedRW = sqrt(m_airspeedRW.x * m_airspeedRW.x + m_airspeedRW.y * m_airspeedRW.y + m_airspeedRW.z * m_airspeedRW.z);
	m_aoaLW = atan2(cross(m_nLW, m_liftVecLW) * tmpNormalLW, m_liftVecLW * m_nLW);
	m_aoaRW = atan2(cross(m_nRW, m_liftVecRW) * tmpNormalRW, m_liftVecRW * m_nRW);

}


//This calculates only aerodynamic forces and moments.
void Skyhawk::FlightModel::calculateAero()
{

	lift();
	drag();
	calculateElements();
	sideForce();
	thrustForce();

	L_stab();
	M_stab();
	N_stab();
	//printf("moment.z: %lf, beta: %lf, m_q: %lf, m_p: %lf, omegax: %lf, omegay: %lf, Cla(mach): %lf\n", 
		//m_moment.z, m_beta, m_q, m_p, m_omega.x, m_omega.y, Cla(m_mach));
}

void Skyhawk::FlightModel::calculateElements()
{
	Vec3 LiftLW = windAxisToBody(m_LDwindAxesLW, m_aoaLW, m_beta);
	Vec3 LiftRW = windAxisToBody(m_LDwindAxesRW, m_aoaRW, m_beta);

	addForceDir(LiftLW, m_rLW);
	addForceDir(LiftRW, m_rRW);
}

//This calculates all forces and moments. Including landing gear.
void Skyhawk::FlightModel::calculateForcesAndMoments(double dt)
{
	//Reset at the start of the frame.
	m_force = Vec3();
	m_moment = Vec3();
	
	calculateLocalPhysicsParams();

	m_aoaDot = (m_aoa - m_aoaPrevious) / dt;

	//Get airspeed and scalar speed squared.
	m_airspeed = m_worldVelocity - m_wind;
	m_scalarVSquared = m_airspeed.x * m_airspeed.x + m_airspeed.y * m_airspeed.y + m_airspeed.z * m_airspeed.z;

	m_scalarV = sqrt(m_scalarVSquared);
	m_mach = m_scalarV / m_speedOfSound;

	m_k = m_scalarVSquared * m_density * 0.5 * m_totalWingArea;
	m_kL = pow(m_scalarAirspeedLW, 2) * m_density * 0.5 * m_totalWingArea;
	m_kR = pow(m_scalarAirspeedRW, 2) * m_density * 0.5 * m_totalWingArea;
	m_q = m_k * m_totalWingSpan;
	m_p = m_scalarV * m_density * 0.25 * m_totalWingArea * m_totalWingSpan * m_totalWingSpan;

	//Slats logic
	double slatPositionL = (m_aoaLW - 0.2268)/0.069813; //full actuation ~17 deg, onset ~13 deg aoa
	double slatPositionR = (m_aoaRW - 0.2268)/0.069813; //full actuation ~17 deg, onset ~13 deg aoa
	slatPositionL = std::min(slatPositionL, 1.0);
	slatPositionL = std::max(slatPositionL, 0.0);
	slatPositionR = std::min(slatPositionR, 1.0);
	slatPositionR = std::max(slatPositionR, 0.0);


	//printf("slats: %lf\n", slatPosition);
	m_controls.slatL() = slatPositionL;
	m_controls.slatR() = slatPositionR;
	
	calculateAero();
	m_aoaPrevious = m_aoa;
}

void Skyhawk::FlightModel::csvData(std::vector<double>& data)
{
	

	
}