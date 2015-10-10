$(document).ready(function(){
	// 评论变量保存
 	var CD = 
	 	{
	 		"articleId"  : $(".articleInfoPack").attr("data-articleId"),
	 		"reviewedId" : $(".authorHead").attr("data-authorId"),
	 		"reviewId"   : 0, /*一级评论ID*/
	 		"replyId"	 : 0, /*二级评论ID*/
	 		"status"   : 0 /*判断依据 0为评论 1为回复评论 2为回复回复*/
	 	};
	$("title").text($(".title").text());
	//加载完毕即刻判断登陆状态
	(function(){
		var _loginStatue = $(".user").attr("name");
		if (_loginStatue){
			$(".loginButton").hide();
			$(".userHead").show();
			// 点赞判定
			isPraised();
			$(".touristPannel").hide();
			$(".memberPannel").show();
			// 回复按钮绑定
			$(".firstReply").on("click",function(){
				loginReview($(this));
			});
			$(".secondReply").on("click",function(){
				loginReply($(this));
			});
			// 取消回复绑定
			$(".mClose").on("click",mCancel);
			// 添加评论绑定
			$("#mAddComment").on("click",mAddComment);
			addTagLogin();
		}
		else{
			$(".loginButton").show();
			$(".userHead").hide();
			// 点赞按钮绑定登陆事件
			goLogin();
			// 回复按钮绑定
			$(".firstReply").on("click",function(){
				review($(this));
			});
			$(".secondReply").on("click",function(){
				reply($(this));
			});
			// 取消回复绑定
			$(".tClose").on("click",tCancel);
			// 添加评论绑定
			$("#tAddComment").on("click",tAddComment);
			addTagLogout();
		}
	}());
	
	


    /************************以上为通用代码*************************/
   // 未登录点赞绑定函数
   function goLogin(){
   		$(".likeUp").bind("click",function(){
   			$(".loginButton").trigger("click");
   		});
   }
   // 标签处理
    (function(){
    	var tags = $(".tagPack").attr("data-tags");
    	tags = tags.split(" , ");
    	var _tagList = $(".tagList");
    	var _demo = $(".tagDemo").children(".tag");
    	for(var i=0,LEN = tags.length; i<LEN; i++){
    		_tagList.append(_demo.clone(true));
    		_tagList.find(".tagContent:last").text(tags[i]);
    	}
    }());
    // 点赞状态判断
    function isPraised(){
			var _like = $("#like");
			if(_like.attr("data-isPraised") == "1"){
					$(".likeUp").css("color","#E74C3C");
			}
			else{
				$(".likeUp").bind("click",getlike);
			}

    }
    // 点赞绑定
    function getlike(){
    	var _likeUp = $(".likeUp");
		var _that = _likeUp.next();
		var urlLike = "/RdcBlog/addPraise.do";
		var _articleId = $(".articleInfoPack").attr("data-articleId");
			myAjax({
				articleId: _articleId
				},urlLike,
				function(data){
					var obj = eval ("(" + data + ")");
					if(obj.status == "success"){
						_likeUp.css("color","#E74C3C");
						_that.text($("#like").next().text()-0+1);
						_likeUp.unbind("click",getlike);
					}
					else{
						_likeUp.css("color","#d0d3d7");
					}
			});
    }
    // 评论时间格式转换函数
    // 2015-08-24 11:25:54 
    // 2015年8月24日 上午11:25
    function transTime(objTime){
    //	var mon = ["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"];
    	var year = objTime.substr(0,4),
    		month = parseInt(objTime.substr(5,2)),
    		day = parseInt(objTime.substr(8,2)),
    		hour = parseInt(objTime.substr(11,2)),
    		min = objTime.substr(14,2);
    	var date = year + "年" + month + "月" + day + "日";
    	var chinese = "";
    	if(hour >= 11){
    		if (hour <= 13) {chinese = "中午";}
    		else if (hour <= 18) {chinese = "下午";}
    		else if (hour <= 22) {chinese = "晚上";}
    		else {chinese = "深夜";}
    		hour = hour > 12 ? hour-12 : hour;
    	}
    	else{
    		if (hour <= 1) {chinese = "深夜";}
    		else if (hour <= 4) {chinese = "凌晨";}
    		else if (hour <= 6) {chinese = "清晨";}
    		else if (hour <= 8) {chinese = "早上";}
    		else {chinese = "上午";}
    	}
    	var time = chinese + hour + ":" + min;
    	return (date + " " + time);
    }
    // 加载完毕即刻转换评论时间
    $(".commentTime").each(function(){
    	var objTime = $(this).text();
    	$(this).text(transTime(objTime));
    });
	// 评论变量修改函数
	function changeCD(status,reviewId,replyId){
		/*  是否回复评论，一级评论ID，二级评论ID */
		CD.status = status;
		CD.reviewId = reviewId;
		CD.replyId = replyId;
	}



	// 登陆后 回复评论 按钮绑定函数 	
	function loginReview(obj){
		changeCD(1,obj.attr("data-reviewId"));
    	var _reviewerNameText = obj.siblings(".reviewerName").text();
    	$(".replyName").text("评论 " + _reviewerNameText);
    	$(".memberReply").show();
    	$(".mMessage").focus();
	}
	// 登陆后 回复回复 按钮绑定函数
	function loginReply(obj){
		var _reviewerFromText = obj.siblings(".reviewerFrom").text();
		changeCD(2,obj.attr("data-reviewId"),obj.attr("data-replyId"));
    	$(".replyName").text("回复 " + _reviewerFromText);
    	$(".memberReply").show();
    	$(".mMessage").focus();
	}
	//  登陆后 取消回复
	function mCancel(){
		$(".memberReply").fadeOut(100);
		changeCD(0);
	}
	//登录后 添加评论 绑定函数
	function mAddComment(){
		var content = tagFilter($(".mMessage").val());
		var	url0 = "/RdcBlog/review.do",
			url1 = "/RdcBlog/reply.do",
			url2 = "/RdcBlog/replyR.do";
		if (!(/\S/.test(content))) {
			tip("评论内容不能为空！");
			return false;
		}
		else if (CD.status == 0) {
			myAjax(
				{
					review : content,
					reviewedId : CD.reviewedId,
					articleId : CD.articleId
				},
				url0,
				function(data){
					var obj = eval ("(" + data + ")");
					if (obj.status == "success"){
						location.reload();
					}
					else{
						tip("评论失败！" + obj.reason);
					}
			});
		}
		else if (CD.status == 1) {
			myAjax(
				{
					reply : content,
					reviewId : CD.reviewId,
					articleId : CD.articleId
				},
				url1,
				function(data){
					var obj = eval ("(" + data + ")");
					if (obj.status == "success"){
						location.reload();
					}
					else{
						tip("评论失败！" + obj.reason);
					}
			});
		}
		else if (CD.status == 2) {
			myAjax(
				{
					reply : content,
					reviewId : CD.reviewId,
					replyId : CD.replyId,
					articleId : CD.articleId
				},
				url2,
				function(data){
					var obj = eval ("(" + data + ")");
					if (obj.status == "success"){
						location.reload();
					}
					else{
						tip("评论失败！" + obj.reason);
					}
			});
		}
		else {tip("评论对象错误！");}


	}



	// 未登录 回复评论 按钮绑定函数 
	function review(obj){
		changeCD(1,obj.attr("data-reviewId"));
    	var _reviewerNameText = obj.siblings(".reviewerName").text();
    	$(".touristReply").text("评论 " + _reviewerNameText);
    	$(".tReply").show();
    	$(".tMessage").focus();
    	$(".touristReplyPack").css("border-bottom","3px solid #CFE8E4");

	}
	// 未登录 回复回复 按钮绑定函数
	function reply(obj){
		var _reviewerFromText = obj.siblings(".reviewerFrom").text();
		changeCD(2,obj.attr("data-reviewId"),obj.attr("data-replyId"));
    	$(".touristReply").text("回复 " + _reviewerFromText);
    	$(".tReply").show();
    	$(".tMessage").focus();
    	$(".touristReplyPack").css("border-bottom","3px solid #CFE8E4");
	}
	//  未登录 取消回复
	function tCancel(){
		$(".tReply").fadeOut(100);
		changeCD(0);
		$(".touristReplyPack").css("border-bottom","3px solid #ececec");
	}
	// 昵称长度限制
	$("#tNickname").change(function(){
		var tempVal = $(this).val();
		var lastText = getStringByteInfo(tempVal,14);
		if(lastText.length > 14){
			$(this).val(lastText.limitStr);
			tip("昵称长度不能超过14个字节（7个汉字）！请重新输入");
		}
	});
	// 未登录 添加评论 绑定函数
	function tAddComment(){
		var nickname = $("#tNickname").val(),
			email = $("#tEmail").val(),
			content = tagFilter($(".tMessage").val()),
			reg = /^[a-z0-9|_]+@[a-z0-9]+(\.(com)|(cn)){1,2}$/i,
			url0 = "/RdcBlog/doReview.do",
			url1 = "/RdcBlog/doReply.do",
			url2 = "/RdcBlog/doReplyR.do";
		if (!(/\S/.test(nickname))) {
			tip("请填写昵称！");
			return false;
		}
		else if (!(/\S/.test(email))) {
			tip("请填写邮箱！");
			return false;
		}
		else if (!(/\S/.test(content))) {
			tip("评论内容不能为空！");
			return false;
		}
		else if (getLenth(nickname)>14) {
			tip("昵称过长！");
			return false;
		}
		else if (!reg.test(email)) {
			tip("邮箱格式不符！");
			return false;
		}
		else{
			nickname += "(游客)";
			if (CD.status == 0) {
				myAjax(
					{
						review : content,
						reviewedId : CD.reviewedId,
						articleId : CD.articleId,
						replier : nickname,
						email : email
					},
					url0,
					function(data){
						var obj = eval ("(" + data + ")");
						if (obj.status == "success"){
							location.reload();
						}
						else{
							tip("评论失败！" + obj.reason);
						}
				});
			}
			else if (CD.status == 1) {
				myAjax(
					{
						reply : content,
						reviewId : CD.reviewId,
						articleId : CD.articleId,
						replier : nickname,
						email : email
					},
					url1,
					function(data){
						var obj = eval ("(" + data + ")");
						if (obj.status == "success"){
							location.reload();
						}
						else{
							tip("评论失败！" + obj.reason);
						}
				});
			}
			else if (CD.status == 2) {
				myAjax(
					{
						reply : content,
						reviewId : CD.reviewId,
						replyId : CD.replyId,
						articleId : CD.articleId,
						replier : nickname,
						email : email
					},
					url2,
					function(data){
						var obj = eval ("(" + data + ")");
						if (obj.status == "success"){
							location.reload();
						}
						else{
							tip("评论失败！" + obj.reason);
						}
				});
			}
			else {tip("评论对象错误！");}
			}
	}
	// 昵称邮箱输入交互
	$(".touristData").keyup(function(){
		var content = $(this).val();
		if (/\S/.test(content)) {
			$(this).css("border-bottom","3px solid #CFE8E4");
		}
		else {
			$(this).css("border-bottom","3px solid #ececec");
		}
	});
	$(".touristData").blur(function(){
		var content = $(this).val();
		if (/\S/.test(content)) {
			$(this).css("border-bottom","3px solid #CFE8E4");
		}
		else {
			$(this).css("border-bottom","3px solid #ececec");
		}
	});





	// 在代码块前加入开关选项
	(function(){
		$(".articleContent pre").each(function(){
			$(this).before($(".codeDemo").children(".codeToggle").clone(true));
		});
	}());
	$(".codeSwitch").on("click",function(){
		var parent = $(this).parent();
		parent.children("p").toggle();
		parent.next().toggle();
	});	





	// 字节长度转换函数
	function getLenth(str){
		return str.replace(/[^\x00-xff]/g,"xx").length;
	}
	// 截取前n个字符
	function getStringByteInfo ( string, intVal ) {
    	var str = $.trim(string) || '';
    	var length = str.length;
    	var len, reg, charStr;
    	var limitStr = '';
        if ( length > 0 ) {
            len = 0;
            reg = /[^\x00-\xff]/;
            for ( var i = 0; i < length; i++) {
                charStr = str.charAt(i);
                if ( reg.test(charStr) ) {
                    len += 2;
                }
                else {
                    len ++;
                }
                if (len <= intVal) {
                    limitStr += str.charAt(i);
                }
            }
        }
        return {
            length: len || 0,
            limitStr: limitStr || ''
        }
    }

    // 过滤评论标签
    function tagFilter(content){
	    // 过滤除a、strong外的其他标签
		var elsetag = /<(?!strong)(?!a)([a-z0-9]+)(\s*[^>]*)\s*>/ig;
		var elsetagEnd = /<\/(?!strong)(?!a)([a-z0-9]+)\s*[^>]*>/ig;
		
		// 过滤a标签除href外的其他属性
		var aAttr = /<a(\s*href="[^"]*")?[^>]*>/ig;
		
		// 过滤strong标签的全部属性
		var strongAttr = /<strong\s*[^>]*>/ig;
		
		content = content
					.replace(/(^\s*)|(\s*$)/g,"") /*去除前后空格*/
					.replace(elsetag,'&lt;$1$2&gt;')
					.replace(elsetagEnd,'&lt;/$1&gt;')
					.replace(aAttr,'<a $1 target="_blank">')
					.replace(strongAttr,'<strong>');
		return content;
    }
    // 评论标签事件
    	// 未登录
    function addTagLogout(){
    	$("#addA").click(function(){
    		$(".tMessage").val($(".tMessage").val() + '<a href=""></a>');
    		$(".tMessage").focus();
    	});
    	$("#addStrong").click(function(){
    		$(".tMessage").val($(".tMessage").val() + '<strong></strong>');
    		$(".tMessage").focus();
    	});
    }
    	// 登陆
    function addTagLogin(){
    	$("#addA").click(function(){
    		$(".mMessage").val($(".mMessage").val() + '<a href=""></a>');
    		$(".mMessage").focus();
    	});
    	$("#addStrong").click(function(){
    		$(".mMessage").val($(".mMessage").val() + '<strong></strong>');
    		$(".mMessage").focus();
    	});
    }



    // 隐藏显示评论切换
     // 欲显示 
    $(".hiden").on("click",function(){
    	$(this).siblings(".commentListSec").slideDown(200);
    	$(this).hide();
    	$(this).next().show();
    });
     // 欲隐藏 
    $(".showed").on("click",function(){
    	$(this).siblings(".commentListSec").slideUp(200);
    	$(this).hide();
    	$(this).prev().show();
    });
});