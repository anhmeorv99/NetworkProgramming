from django.conf.urls import url, include
from django.contrib import admin
from rest_framework_nested import routers

from chat_app_service.views.users import UserViewSet
from chat_app_service.views.room import RoomViewSet
from chat_app_service.views.message import MessageViewSet
from chat_app_service.views.friends import FriendsViewSet
from chat_app_service.views.chatprivate import ChatPrivateViewSet


class CustomDefaultRouter(routers.DefaultRouter):
    def get_api_root_view(self, api_urls=None):
        return self.APIRootView.as_view(api_root_dict={})


router = CustomDefaultRouter()

router.register(r'api/user', UserViewSet, basename='name')
router.register(r'api/room', RoomViewSet, basename='name')
router.register(r'api/message', MessageViewSet, basename='name')
router.register(r'api/friends', FriendsViewSet, basename='name')
router.register(r'api/private', ChatPrivateViewSet, basename='name')

urlpatterns = [
    url(r'^', include(router.urls)),
    url(r'^admin/', admin.site.urls),
]
