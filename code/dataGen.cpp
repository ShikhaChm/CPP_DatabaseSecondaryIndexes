/*
In this file we will create a database with desired size at the desired path, as indicated by the arguments
The schema of table is  <member_id, sex,ethnicity,education,industry,familySize, salary,age,netWorth ,location> 
*/
#include<iostream>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include"utils.h"
using namespace std;
int main(int argc, char *argv[]) {
	int numDataPoints = atoi(argv[1]);
	string dataPath = argv[2];
	cout << "numDataPoints = " << numDataPoints<<endl<<"DataPath="<<dataPath << endl;
	//Cardinality 2
	string sex[] = {"MALE","FEMALE"};
	//Cardinality 4
	string ethnicity[]={"ASIAN","AFRICAN","EUROPEAN","AMERICAN"};
	//Cardinality 16
	string education[]={"BS","MS","PHD","MBBS","MD","MBA","M.Tech","B.Tech","C.A","BAMS","B.Sc","M.Sc","Law","PhDr.","RNDr.","MDDr."};
	//Cardinality 32
	string industry[]={"SOFTWARE","FINANCE","CIVIL","AVIATION","DEFENCE","MEDICINE","MANUFACTURING","CONSUMERGOODS","AGRICULTURE","OIL & GAS","CHEMICALS","HOUSEWARES","TEXTILE","AUTO PARTS","ELECTRONICS","APPLIANCES","CONFECTIONERS","HOTEL","COSMETICS","(IT)HARDWARE","B.P.O","INSURANCE","ENERGY","UTILITIES","REAL ESTATE","BANKS","TELECOM","TRAVEL & LEISURE","RETAIL","HEALTH CARE","FOOD & BEVERAGE","AUTOMOBILES"};
	//Cardinality 16
	int familySize[16];
	for(int i=0;i<16;i++) familySize[i]=1+i;
	//Cardinality 1024
	int salary[256];
	for(int i=0;i<256;i++) salary[i]=i;//USD
	//Cardinality 32
	int age[32];
	for(int i=0;i<32;i++) age[i]=i;//years
	//Cardinality 256 
	int netWorth[512];
	for(int i=0;i<512;i++) netWorth[i]=1000*(1+ i);//USD
	//cardinality 512 
	string location[]={"ALABAMA","ALASKA","AMERICAN Samoa","ARIZONA","ARKANSAS","CALIFORNia","COLORADO","CONNECTIcut","DELAWARE","DISTRICT of Columbia","FLORIDA","GEORGIA","GUAM","HAWAII","IDAHO","ILLINOIS","INDIANA","IOWA","KANSAS","KENTUCKY","LOUISIANa","MAINE","MARYLAND","MASSACHUsetts","MICHIGAN","MINNESOTa","MISSISSIppi","MISSOURI","MONTANA","NEBRASKA","NEVADA","NEW HAMPshire","NEW JERSey","NEW MEXIco","NEW YORK","NORTH CArolina","NORTH DAkota","NORTHERN Marianas Islands","OHIO","OKLAHOMA","OREGON","PENNSYLVania","PUERTO Rico","RHODE ISland","SOUTH CArolina","SOUTH DAkota","TENNESSEe","TEXAS","UTAH","VERMONT","VIRGINIA","VIRGIN Islands","WASHINGTon","WEST VIRginia","WISCONSIn","WYOMING","Kolhapur","Port Blair","Adilabad	","	Adoni	","	Amadalavalasa	","	Amalapuram	","	Anakapalle	","	Farooqnagar	","	Gadwal	","	Gooty	","	Gudivada	","	Gudur	","	Guntakal	","	Guntur	","	Hanuman Junction	","	Hindupur	","	Hyderabad	","	Ichchapuram	","	Jaggaiahpet	","	Jagtial	","	Jammalamadugu	","	Jangaon	","	Kadapa	","	Kadiri	","	Busan	","	Kothapeta	","	Kovvur	","	Kurnool	","	Kyathampalle	","	Macherla	","	Machilipatnam	","	Madanapalle	","	Mahbubnagar	","	Mancherial	","	Mandamarri	","	Mandapeta	","	Manuguru	","	Markapur	","	Medak	","	Miryalaguda	","	Mogalthur	","	Nagari	","	Nagarkurnool	","	Nandyal	","	Narasapur	","	Narasaraopet	","	Narayanpet	","	Narsipatnam	","	Nellore	","	Nidadavole	","	Nirmal	","	Nizamabad	","	Nuzvid	","	Ongole	","	Palacole	","	Palasa Kasibugga	","	Palwancha	","	Parvathipuram	","	Pedana	","	Peddapuram	","	Pithapuram	","	Pondur	","	Ponnur	","	Proddatur	","	Punganur	","	Puttur	","	Rajahmundry	","	Rajam	","	Ramachandrapuram	","	Ramagundam	","	Rayachoti	","	Rayadurg	","	Renigunta	","	Repalle	","	Sadasivpet	","	Salur	","	Samalkot	","	Sangareddy	","	Sattenapalle	","	Siddipet	","	Singapur	","	Sircilla	","	Srikakulam	","	Srikalahasti	","	Suryapet	","	Tadepalligudem	","	Tadpatri	","	Tandur	","	Tanuku	","	Tenali	","	Tirupati	","	Tuni	","	Uravakonda	","	Venkatagiri	","	Vicarabad	","	Vijayawada	","	Vinukonda	","	Visakhapatnam	","	Vizianagaram	","	Wanaparthy	","Warangal","Yellandu","	Yemmiganur	","	Yerraguntla	","	Zahirabad	","	Rajampet	","	Along	","	Bomdila	","	Itanagar	","	Naharlagun	","	Pasighat	","	Abhayapuri	","	Amguri	","	Anandnagaar	","	Barpeta	","	Barpeta Road	","	Karimganj	","	Kokrajhar	","	Lanka	","	Lumding	","	Mangaldoi	","	Mankachar	","	Margherita	","	Mariani	","	Marigaon	","	Nagaon	","	Nalbari	","	North Lakhimpur	","Rangia","Sibsagar","Silchar","	Tezpur	","	Tinsukia	","	Amarpur	","	Araria	","	Areraj	","	Arrah	","	Asarganj	","	Aurangabad	","	Bagaha	","	Bahadurganj	","	Bairgania	","	Bakhtiarpur	","	Banka	","	Banmankhi Bazar	","	Barahiya	","	Barauli	","	Barbigha	","	Barh	","	Begusarai	","	Behea	","	Bettiah	","	Bhabua	","	Bhagalpur	","	Bihar Sharif	","	Bikramganj	","	Bodh Gaya	","	Buxar	","	Chandan Bara	","	Chanpatia	","	Chhapra	","	Colgong	","	Dalsinghsarai	","	Darbhanga	","	Daudnagar	","	Dehri-on-Sone	","	Dhaka	","	Dighwara	","	Dumraon	","	Fatwah	",
"Forbesganj","Kharagpur	","	Kishanganj	","	Lakhisarai	","	Lalganj	","	Madhepura	","	Madhubani	","	Maharajganj	","	Mahnar Bazar	","	Makhdumpur	","	Maner	","	Manihari	","	Marhaura	","	Masaurhi	","	Mirganj	","	Mokameh	","	Motihari	","	Motipur	","	Munger	","	Murliganj	","	Muzaffarpur	","	Narkatiaganj	","	Sheohar	","	Sherghati	","	Silao	","	Sitamarhi	","	Siwan	","	Sonepur	","	Sugauli	","	Sultanganj	","	Supaul	","	Warisaliganj	","	Ahiwara	","	Akaltara	","	Ambagarh Chowki	","	Ambikapur	","	Arang	","	Bade Bacheli	","	Balod	","	Mahendragarh	","	Mungeli	","	Naila Janjgir	","	Raigarh	","	Raipur	","	Rajnandgaon	","	Sakti	","	Tilda Newra	","	Amli	","	Silvassa	","	Daman and Diu	","	Daman and Diu	","	Asola	","	Delhi	","	Aldona	","	Curchorem Cacora	","	Madgaon	","	Mapusa	","	Margao	","	Marmagao	","	Panaji	","	Ahmedabad	","	Kadi	","	Kalavad	","	Kalol	","	Kapadvanj	","	Karjan	","	Keshod	","	Khambhalia	","	Khambhat	","	Kheda	","	Khedbrahma	","	Kheralu	","	Kodinar	","	Lathi	","	Limbdi	","	Lunawada	","	Mahesana	","	Mahuva	","	Manavadar	","	Mandvi	","	Mangrol	","	Mansa	","	Mehmedabad	","	Modasa	","	Morvi	","	Nadiad	","	Navsari	","	Padra	","	Palanpur	","	Palitana	","	Pardi	","	Patan	","	Petlad	","	Porbandar	","	Radhanpur	","	Rajkot	","	Rajpipla	","	Rajula	","	Ranavav	","	Rapar	","	Salaya	","	Sanand	","	Savarkundla	","	Sidhpur	","	Sihor	","	Songadh	","	Surat	","	Talaja	","	Thangadh	","	Tharad	","	Umbergaon	","	Umreth	","	Una	","	Unjha	","	Upleta	","	Vadnagar	","	Vadodara	","	Valsad	","	Vapi	","	Vapi	","	Veraval	","	Vijapur	","	Viramgam	","	Visnagar	","	Vyara	","	Wadhwan	","	Wankaner	","	Adalaj	","	Adityana	","	Alang	","	Ambaji	","	Ambaliyasan	","	Andada	","	Anjar	","	Anklav	","	Antaliya	","	Arambhada	","	Atul	","	Ballabhgarh	","	Ambala	","Ambala"," Madhupur	","	Mihijam	","	Musabani	","	Pakaur	","	Khunti	","	Lohardaga	","	Hisar	","	Hodal	","	Jhajjar	","	Jind	","	Kaithal	","	Kalan Wali	","	Kalka	","	Karnal	","	Ladwa	","	Mahendragarh	","	Mandi Dabwali	","	Narnaul	","	Narwana	","	Palwal	","	Panchkula	","	Panipat	","	Pehowa	","	Pinjore	","	Rania	","	Ratia	","	Rewari	","	Rohtak	","	Safidon	","	Samalkha	","	Shahbad	","	Sirsa	","	Sohna	","	Sonipat	","	Taraori	","	Thanesar	","	Tohana	","	Yamunanagar	","	Arki	","	Buddi	","	Bilaspur	","	Chamba	","	Dalhousie	","	Dharamsala	","	Hamirpur	","	Mandi	","	Nahan	","	Shimla	","	Solan	","	Sundarnagar	","	Jammu	","	Achabbal	","	Akhnoor	","	Anantnag	","	Arnia	","	Awantipora	","	Bandipore	","	Baramula	","	Kathua	","	Leh	","	Punch	","	Rajauri	","	Sopore	","	Srinagar	","	Udhampur	","	Amlabad	","	Ara	","	Barughutu	","	Bokaro Steel City	","	Sagar	","	Sakleshpur	","	Sandur	","	Sankeshwar	","	Saundatti-Yellamma	","	Savanur	","	Sedam	","	Shahabad	","	Shahpur	","	Chuncheon	","	Shikapur	","	Shimoga	","	Shorapur	","	Shrirangapattana	","	Sidlaghatta	","	Sindgi	","	Sindhnur	","	Sira	","	Sirsi	","	Siruguppa	","	Mattannur	","	Mavelikkara	","	Mavoor	","	Muvattupuzha	","	Nedumangad	","	Neyyattinkara	","	Ottappalam	","	Palai	","	Palakkad	","Cherthala" ,
"Cherutazham","Chittur-Thathamangalam	","	Chockli	","	Erattupetta	","	Guruvayoor	","	Irinjalakuda	","	Kadirur	","	Ashok Nagar	","	Balaghat	","	Quilandy	","	Shoranur	","	Taliparamba	","	Thiruvalla	","	Thiruvananthapuram	","	Thodupuzha	","	Thrissur	","	Tirur	","	Vadakara	","	Vaikom	","	Varkala	","	Kavaratti	","	Daejeon	","	Gwangju	","	Gyeonggi Province	","	Ulsan	","	Incheon	","	Daegu	","	Goyang	","	Yongin	","	Bucheon	","	Ansan	","	Cheongju","	Jeonju	","	Anyang	","	Cheonan	","	Namyangju","	Pohang	","	Gimhae	","	Hwaseong","	Uijeongbu","	Siheung	","	Gumi	","	Jeju-si	","	Pyeongtaek	","	Jinju	","	Gwangmyeong	","	Paju	"," Calcutta ","	Wonju	","	Iksan	","	Asan	","Gunpo","New-Delhi"};


	//cout << location[3] << "  " << age[10]<< "  " << netWorth[10]<<"  "<< salary[10] << "  " << familySize[5] << "  "<<getRand(5,50)<<endl;
	// Now we will create data files
	string dataFilePath= dataPath+"/dataFile.csv";	
	string binDataPath=dataPath+"/bin";
	system(("rm -rf "+dataPath).c_str());
	system(("mkdir "+dataPath).c_str());
	//system(("mkdir "+binDataPath).c_str());
	cout << dataFilePath << endl;
	ofstream dataFile;
	dataFile.open(dataFilePath.c_str());
	dataFile << "MemberId,Sex,Ethnicity,Education,Industry,FamilySize,Salary,Age,NetWorth,Location"<<endl;
	for(int i=0;i<numDataPoints;i++) {
		stringstream stringStream;
		stringStream << i<<","<<sex[getRand(0,1)]<<","<<ethnicity[getRand(0,3)]<<","<<education[getRand(0,15)]<<","<<industry[getRand(0,31)]<<","<<familySize[getRand(0,15)]<<","<<salary[getRand(0,255)]<<","<<age[getRand(0,31)]<<","<<netWorth[getRand(0,511)]<<","<<location[getRand(0,511)];

		string recordData = stringStream.str();
		dataFile << recordData << endl;
		/*
		char fileName[100];
		sprintf(fileName,"%s/%i",binDataPath.c_str(),i);
		ofstream record;
		record.open(fileName);
		record << recordData <<endl ;
		//ofstream record(fileName, ios::out | ios::binary);
		//record.write(recordData.c_str(),sizeof(recordData));
		record.close();
		*/
	}
	dataFile.close();
	return 0;
}
