#include "pinyin.h"
#include "config.h"
#include <string.h>

struct PinyinEntry {
    const char* pinyin;
    const char* candidates;
};

static const PinyinEntry dict[] PROGMEM = {
    {"a",     "啊阿呵嗄吖"},
    {"ai",    "爱矮挨唉碍癌艾"},
    {"an",    "安暗按岸案俺"},
    {"ang",   "昂盎肮"},
    {"ao",    "奥熬傲凹懊澳"},
    {"ba",    "八把爸吧拔罢霸坝"},
    {"bai",   "白百拜败摆柏伯"},
    {"ban",   "半办班般搬板版伴扮"},
    {"bang",  "帮棒傍膀绑榜磅"},
    {"bao",   "包报保抱暴宝薄饱爆"},
    {"bei",   "北被背杯备悲辈碑"},
    {"ben",   "本笨奔苯"},
    {"beng",  "蹦崩绷泵甭"},
    {"bi",    "比笔必避壁鼻毕闭逼币"},
    {"bian",  "变边便遍编辨辩鞭扁"},
    {"biao",  "表标彪膘"},
    {"bie",   "别憋鳖"},
    {"bin",   "宾滨彬斌濒"},
    {"bing",  "并兵病冰饼丙柄"},
    {"bo",    "波拨博伯薄播脖"},
    {"bu",    "不步布部补捕卜"},
    {"ca",    "擦嚓"},
    {"cai",   "才材财彩菜猜裁踩"},
    {"can",   "参残惨灿蚕餐"},
    {"cang",  "藏仓苍沧舱"},
    {"cao",   "草操曹槽糙"},
    {"ce",    "册侧策测厕"},
    {"ceng",  "层曾蹭"},
    {"cha",   "查差茶插叉察岔"},
    {"chai",  "拆柴豺"},
    {"chan",  "产缠铲掺搀蝉"},
    {"chang", "长常场唱厂尝偿畅"},
    {"chao",  "超朝潮吵抄炒巢"},
    {"che",   "车彻扯撤"},
    {"chen",  "陈沉晨臣称趁衬"},
    {"cheng", "成城程承诚称乘撑"},
    {"chi",   "吃尺迟池翅齿耻斥"},
    {"chong", "冲充虫崇宠重"},
    {"chou",  "抽仇愁丑臭筹酬"},
    {"chu",   "出处初除楚础触储"},
    {"chuang","窗床创闯吹"},
    {"chui",  "吹垂炊锤"},
    {"chun",  "春纯唇蠢醇"},
    {"chuo",  "戳绰龊"},
    {"ci",    "此次词刺瓷辞慈"},
    {"cong",  "从聪丛匆葱囱"},
    {"cou",   "凑辏腠"},
    {"cu",    "粗促醋簇猝"},
    {"cuan",  "窜篡攒"},
    {"cui",   "催脆翠崔摧粹"},
    {"cun",   "村存寸忖"},
    {"cuo",   "错措搓挫磋"},
    {"da",    "大打达答搭瘩"},
    {"dai",   "大带代待袋戴呆"},
    {"dan",   "但单担蛋淡弹胆旦"},
    {"dang",  "当党挡档荡"},
    {"dao",   "到道刀倒导岛盗稻"},
    {"de",    "的地得德"},
    {"deng",  "等灯登邓凳瞪"},
    {"di",    "的地低底敌第帝递"},
    {"dian",  "点电店典垫殿淀碘"},
    {"diao",  "掉调吊钓雕叼"},
    {"die",   "跌爹叠碟蝶谍"},
    {"ding",  "定顶丁盯钉订鼎"},
    {"diu",   "丢"},
    {"dong",  "东动冬洞懂冻栋"},
    {"dou",   "都斗豆逗兜陡抖"},
    {"du",    "都读度独毒堵赌杜"},
    {"duan",  "段短断端锻缎"},
    {"dui",   "对队堆兑"},
    {"dun",   "顿蹲盾吨敦墩"},
    {"duo",   "多朵夺躲堕舵剁"},
    {"e",     "恶饿额鹅俄蛾愕"},
    {"en",    "恩嗯摁"},
    {"er",    "二而耳儿尔饵"},
    {"fa",    "发法罚伐阀乏"},
    {"fan",   "反饭翻犯番凡烦范"},
    {"fang",  "方放房防访仿纺芳"},
    {"fei",   "非飞费废肥匪肺"},
    {"fen",   "分份粉奋愤坟纷芬"},
    {"feng",  "风封丰逢峰锋疯枫"},
    {"fo",    "佛"},
    {"fou",   "否"},
    {"fu",    "父夫服福负付附府复"},
    {"ga",    "嘎噶尬"},
    {"gai",   "该改盖概溉钙"},
    {"gan",   "干感赶敢甘肝杆柑"},
    {"gang",  "刚钢港岗杠缸"},
    {"gao",   "高告搞稿糕膏"},
    {"ge",    "个各歌哥割革隔阁"},
    {"gei",   "给"},
    {"gen",   "跟根"},
    {"geng",  "更耕梗庚羹"},
    {"gong",  "工公共功攻供宫弓"},
    {"gou",   "够狗沟构钩勾苟"},
    {"gu",    "古故顾鼓骨谷股固"},
    {"gua",   "挂瓜刮寡卦褂"},
    {"guai",  "怪乖拐"},
    {"guan",  "关管官观馆惯灌贯"},
    {"guang", "光广逛"},
    {"gui",   "贵归鬼规桂柜跪"},
    {"gun",   "滚棍"},
    {"guo",   "过国果裹锅"},
    {"ha",    "哈蛤"},
    {"hai",   "还海孩害嗨骸"},
    {"han",   "汉喊寒含旱汗韩焊"},
    {"hang",  "行航杭巷"},
    {"hao",   "好号豪毫耗浩皓"},
    {"he",    "和合何河喝贺赫荷"},
    {"hei",   "黑嘿"},
    {"hen",   "很恨狠痕"},
    {"heng",  "横衡恒哼"},
    {"hong",  "红宏洪虹鸿轰哄"},
    {"hou",   "后候厚侯猴喉"},
    {"hu",    "湖呼胡护虎互户忽"},
    {"hua",   "花话化华画划滑"},
    {"huai",  "坏怀淮槐"},
    {"huan",  "还换欢环患幻唤焕"},
    {"huang", "黄皇荒慌晃恍幌"},
    {"hui",   "会回灰汇挥毁辉恢"},
    {"hun",   "混魂昏婚浑"},
    {"huo",   "活火或货获伙惑"},
    {"ji",    "几机及级急集即己记"},
    {"jia",   "家加假价甲驾架嫁"},
    {"jian",  "见间件建简检减坚剑"},
    {"jiang", "将讲江奖降酱僵姜"},
    {"jiao",  "叫教交觉角脚较焦"},
    {"jie",   "结界接解姐借届街"},
    {"jin",   "进近今金紧仅尽劲"},
    {"jing",  "经精京景静敬净竟"},
    {"jiong", "窘炯"},
    {"jiu",   "就九久旧酒救纠究"},
    {"ju",    "局举巨具句聚拒距"},
    {"juan",  "卷倦绢捐眷"},
    {"jue",   "觉决绝掘爵抉"},
    {"jun",   "军均君俊峻骏"},
    {"ka",    "卡咖喀"},
    {"kai",   "开凯慨楷"},
    {"kan",   "看砍刊堪勘坎"},
    {"kang",  "抗扛康炕糠"},
    {"kao",   "考靠烤拷"},
    {"ke",    "可课科客克刻渴颗"},
    {"ken",   "肯啃恳垦"},
    {"keng",  "坑吭"},
    {"kong",  "空控孔恐"},
    {"kou",   "口扣寇"},
    {"ku",    "苦哭库酷裤窟"},
    {"kua",   "夸跨垮挎"},
    {"kuai",  "快块筷会"},
    {"kuan",  "宽款"},
    {"kuang", "况矿框狂筐旷"},
    {"kui",   "亏愧溃葵魁"},
    {"kun",   "困昆捆"},
    {"kuo",   "扩括阔廓"},
    {"la",    "拉啦辣蜡腊垃"},
    {"lai",   "来赖莱"},
    {"lan",   "蓝兰烂拦懒栏揽"},
    {"lang",  "浪郎朗廊狼琅"},
    {"lao",   "老劳牢捞佬姥"},
    {"le",    "了乐勒"},
    {"lei",   "类累雷泪磊蕾"},
    {"leng",  "冷愣楞"},
    {"li",    "里力理立利离例历"},
    {"lia",   "俩"},
    {"lian",  "连脸练联恋炼链"},
    {"liang", "两亮量凉粮梁辆"},
    {"liao",  "了料聊疗辽燎"},
    {"lie",   "列烈裂劣猎"},
    {"lin",   "林临邻淋琳磷"},
    {"ling",  "令领零灵岭铃龄"},
    {"liu",   "六流留刘柳溜"},
    {"long",  "龙笼隆拢垄弄"},
    {"lou",   "楼漏露搂篓"},
    {"lu",    "路陆露录鹿炉鲁"},
    {"lv",    "绿律率旅虑滤驴"},
    {"luan",  "乱卵峦"},
    {"lue",   "略掠"},
    {"lun",   "论轮伦沦"},
    {"luo",   "落罗螺洛络骆"},
    {"ma",    "妈马吗骂麻码玛"},
    {"mai",   "买卖迈麦埋脉"},
    {"man",   "满慢漫蛮瞒蔓"},
    {"mang",  "忙盲茫芒"},
    {"mao",   "毛猫冒帽矛茂贸"},
    {"me",    "么"},
    {"mei",   "没美每煤梅媒眉"},
    {"men",   "门们闷"},
    {"meng",  "猛蒙盟梦萌孟"},
    {"mi",    "米密迷蜜眯秘觅"},
    {"mian",  "面免棉眠绵缅"},
    {"miao",  "秒妙苗描庙瞄渺"},
    {"mie",   "灭蔑"},
    {"min",   "民敏闽皿"},
    {"ming",  "名明命鸣铭冥"},
    {"mo",    "没磨末模摸莫默魔"},
    {"mou",   "某谋牟眸"},
    {"mu",    "木目母牧墓幕慕"},
    {"na",    "那拿哪呐纳娜"},
    {"nai",   "奶耐乃奈"},
    {"nan",   "南男难"},
    {"nang",  "囊"},
    {"nao",   "脑闹恼挠"},
    {"ne",    "呢讷"},
    {"nei",   "内"},
    {"nen",   "嫩"},
    {"neng",  "能"},
    {"ni",    "你泥逆拟腻尼"},
    {"nian",  "年念粘碾"},
    {"niang", "娘酿"},
    {"niao",  "鸟尿"},
    {"nie",   "捏聂涅镍"},
    {"nin",   "您"},
    {"ning",  "宁凝拧柠"},
    {"niu",   "牛扭纽钮"},
    {"nong",  "农浓弄脓"},
    {"nu",    "努怒奴"},
    {"nv",    "女"},
    {"nuan",  "暖"},
    {"nuo",   "挪诺懦糯"},
    {"o",     "哦噢"},
    {"ou",    "偶欧殴呕鸥"},
    {"pa",    "怕爬帕趴琶"},
    {"pai",   "排拍派牌迫"},
    {"pan",   "盘判盼攀叛"},
    {"pang",  "旁胖庞乓傍"},
    {"pao",   "跑炮抛泡袍咆"},
    {"pei",   "配陪赔培佩沛"},
    {"pen",   "盆喷"},
    {"peng",  "朋碰捧棚膨蓬篷"},
    {"pi",    "皮批匹劈脾疲僻屁"},
    {"pian",  "片偏篇骗翩"},
    {"piao",  "票漂飘瓢"},
    {"pie",   "撇瞥"},
    {"pin",   "品拼贫频聘"},
    {"ping",  "平评瓶凭屏萍苹"},
    {"po",    "破迫泼颇婆坡"},
    {"pou",   "剖"},
    {"pu",    "普铺扑扑葡仆朴"},
    {"qi",    "七起其气期奇齐器"},
    {"qia",   "恰掐洽"},
    {"qian",  "前千钱签浅欠牵铅"},
    {"qiang", "强枪墙抢腔呛"},
    {"qiao",  "桥巧悄敲瞧翘俏"},
    {"qie",   "切且窃茄"},
    {"qin",   "亲琴勤侵秦寝"},
    {"qing",  "请清情青轻庆倾晴"},
    {"qiong", "穷琼"},
    {"qiu",   "球求秋丘邱囚"},
    {"qu",    "去取区趣曲驱渠"},
    {"quan",  "全权劝圈拳泉券"},
    {"que",   "却确缺雀瘸"},
    {"qun",   "群裙"},
    {"ran",   "然染燃"},
    {"rang",  "让嚷壤"},
    {"rao",   "绕扰饶"},
    {"re",    "热惹"},
    {"ren",   "人认任忍仁韧"},
    {"reng",  "仍扔"},
    {"ri",    "日"},
    {"rong",  "容荣融溶蓉绒"},
    {"rou",   "肉柔揉"},
    {"ru",    "如入乳辱儒"},
    {"ruan",  "软"},
    {"rui",   "瑞锐"},
    {"run",   "润闰"},
    {"ruo",   "若弱"},
    {"sa",    "撒洒萨"},
    {"sai",   "赛塞腮"},
    {"san",   "三散伞"},
    {"sang",  "桑嗓丧"},
    {"sao",   "扫嫂骚"},
    {"se",    "色涩瑟"},
    {"sen",   "森"},
    {"seng",  "僧"},
    {"sha",   "杀沙纱傻啥砂"},
    {"shai",  "晒筛"},
    {"shan",  "山善闪衫扇删珊"},
    {"shang", "上商伤赏尚裳"},
    {"shao",  "少烧绍哨稍韶"},
    {"she",   "社设射蛇舍涉奢"},
    {"shei",  "谁"},
    {"shen",  "什深身神审伸甚"},
    {"sheng", "生声省胜升圣剩"},
    {"shi",   "是十时事使实世石"},
    {"shou",  "手受首收守瘦寿"},
    {"shu",   "书数树术输属熟束"},
    {"shua",  "刷耍"},
    {"shuai", "帅摔甩衰率"},
    {"shuan", "栓拴"},
    {"shuang","双爽霜"},
    {"shui",  "水谁睡税"},
    {"shun",  "顺瞬吮"},
    {"shuo",  "说硕朔"},
    {"si",    "四死思丝私似寺撕"},
    {"song",  "送松宋颂耸讼"},
    {"sou",   "搜艘擞"},
    {"su",    "素速苏诉俗宿肃"},
    {"suan",  "算酸蒜"},
    {"sui",   "虽随岁碎穗隧"},
    {"sun",   "孙损笋"},
    {"suo",   "所缩锁索"},
    {"ta",    "他她它塔踏塌"},
    {"tai",   "太台态抬泰胎"},
    {"tan",   "谈弹坦探摊滩贪"},
    {"tang",  "堂糖汤躺趟烫塘"},
    {"tao",   "套逃桃讨淘陶"},
    {"te",    "特"},
    {"teng",  "疼腾藤"},
    {"ti",    "体提题替踢梯剔"},
    {"tian",  "天田甜添填"},
    {"tiao",  "条跳调挑挑"},
    {"tie",   "铁贴帖"},
    {"ting",  "听停挺庭厅亭"},
    {"tong",  "同通痛统铜桶童"},
    {"tou",   "头投透偷"},
    {"tu",    "土图突吐途涂兔"},
    {"tuan",  "团湍"},
    {"tui",   "推退腿蜕"},
    {"tun",   "吞屯囤"},
    {"tuo",   "他拖脱托驼妥驮"},
    {"wa",    "哇挖蛙瓦袜洼"},
    {"wai",   "外歪"},
    {"wan",   "万完玩晚碗弯湾"},
    {"wang",  "王望网忘往旺汪"},
    {"wei",   "为位未围味微危"},
    {"wen",   "问文闻温稳吻纹"},
    {"weng",  "翁嗡"},
    {"wo",    "我握窝卧蜗"},
    {"wu",    "五无物务武午误悟"},
    {"xi",    "西系细洗喜戏希吸"},
    {"xia",   "下夏吓虾瞎峡侠"},
    {"xian",  "先现线限显险仙"},
    {"xiang", "想向象像响乡相箱"},
    {"xiao",  "小笑校消效晓销"},
    {"xie",   "写些谢鞋血协斜"},
    {"xin",   "心新信辛欣薪"},
    {"xing",  "行性星形型兴幸"},
    {"xiong", "兄胸凶雄熊"},
    {"xiu",   "修休秀袖绣锈"},
    {"xu",    "需许续须虚序叙绪"},
    {"xuan",  "选宣旋悬玄绚"},
    {"xue",   "学雪血穴靴"},
    {"xun",   "训寻讯迅巡逊"},
    {"ya",    "呀压牙亚雅崖芽"},
    {"yan",   "眼言烟严沿演验盐"},
    {"yang",  "样阳养洋央仰痒"},
    {"yao",   "要药摇遥咬耀腰"},
    {"ye",    "也业夜叶野页爷"},
    {"yi",    "一以已意义亿易"},
    {"yin",   "因音引银印阴隐"},
    {"ying",  "应影英营迎硬映"},
    {"yo",    "哟"},
    {"yong",  "用永勇拥涌咏泳"},
    {"you",   "有又由右油游友"},
    {"yu",    "于与雨鱼语遇玉"},
    {"yuan",  "远元原园圆员愿"},
    {"yue",   "月越约乐阅悦"},
    {"yun",   "云运允韵孕蕴"},
    {"za",    "杂砸咋"},
    {"zai",   "在再灾载栽宰"},
    {"zan",   "咱赞暂攒"},
    {"zang",  "脏葬赃"},
    {"zao",   "早造遭糟灶燥"},
    {"ze",    "则责择泽"},
    {"zei",   "贼"},
    {"zen",   "怎"},
    {"zeng",  "增曾赠憎"},
    {"zha",   "扎炸渣榨眨诈"},
    {"zhai",  "宅窄债摘斋"},
    {"zhan",  "站战占展沾粘盏"},
    {"zhang", "长张掌丈章涨障丈"},
    {"zhao",  "找照赵招着朝罩"},
    {"zhe",   "这着者折哲遮"},
    {"zhei",  "这"},
    {"zhen",  "真镇阵针振震诊"},
    {"zheng", "正整政争证征症"},
    {"zhi",   "之只知直到值纸"},
    {"zhong", "中种重终众忠钟"},
    {"zhou",  "周洲州舟轴皱粥"},
    {"zhu",   "主住注猪竹祝著柱"},
    {"zhua",  "抓"},
    {"zhuai", "拽"},
    {"zhuan", "转专砖赚撰"},
    {"zhuang","装状壮撞庄幢"},
    {"zhui",  "追坠缀赘"},
    {"zhun",  "准谆"},
    {"zhuo",  "着桌捉拙灼琢"},
    {"zi",    "子自字紫资仔"},
    {"zong",  "总宗综纵踪棕"},
    {"zou",   "走奏揍"},
    {"zu",    "足组族祖阻租"},
    {"zuan",  "钻攥"},
    {"zui",   "最嘴醉罪"},
    {"zun",   "尊遵"},
    {"zuo",   "做作坐座左昨"},
    {NULL, NULL}
};

static char _buffer[MAX_PINYIN_LEN + 1] = {0};
static int _bufLen = 0;
static bool _active = false;

void pinyinInit() {
    _bufLen = 0;
    _buffer[0] = 0;
    _active = false;
}

bool pinyinIsActive() {
    return _active;
}

bool pinyinAddChar(char c) {
    if (c < 'a' || c > 'z') return false;
    if (_bufLen >= MAX_PINYIN_LEN) return false;
    _buffer[_bufLen++] = c;
    _buffer[_bufLen] = 0;
    _active = true;
    return true;
}

bool pinyinBackspace() {
    if (_bufLen <= 0) return false;
    _bufLen--;
    _buffer[_bufLen] = 0;
    if (_bufLen == 0) _active = false;
    return true;
}

const char* pinyinGetText() {
    return _buffer;
}

static int utf8CharLen(const char* s) {
    unsigned char c = (unsigned char)s[0];
    if (c < 0x80) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

int pinyinGetCandidates(const char** outCandidates, int maxCount) {
    if (_bufLen == 0) return 0;
    for (int i = 0; dict[i].pinyin != NULL; i++) {
        if (strcmp(dict[i].pinyin, _buffer) == 0) {
            const char* cands = dict[i].candidates;
            int count = 0;
            int pos = 0;
            while (cands[pos] != 0 && count < maxCount) {
                outCandidates[count] = &cands[pos];
                int len = utf8CharLen(&cands[pos]);
                pos += len;
                count++;
            }
            return count;
        }
    }
    for (int i = 0; dict[i].pinyin != NULL; i++) {
        int pLen = strlen(dict[i].pinyin);
        if (pLen >= _bufLen && strncmp(dict[i].pinyin, _buffer, _bufLen) == 0 && pLen == _bufLen) {
            continue;
        }
    }
    return 0;
}

bool pinyinSelectCandidate(int index, char* outUtf8, int outLen) {
    const char* candidates[MAX_CANDIDATES];
    int count = pinyinGetCandidates(candidates, MAX_CANDIDATES);
    if (index < 0 || index >= count) return false;
    int charLen = utf8CharLen(candidates[index]);
    if (charLen >= outLen) return false;
    memcpy(outUtf8, candidates[index], charLen);
    outUtf8[charLen] = 0;
    return true;
}

void pinyinClear() {
    _bufLen = 0;
    _buffer[0] = 0;
    _active = false;
}
